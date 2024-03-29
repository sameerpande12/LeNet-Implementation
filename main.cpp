#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <algorithm>
#include <vector>
#include<chrono>
#include "activations.h"
#include "pooling.h"
#include "sigmoid.h"
#include "softmax.h"
#include "convolute_mkl.h"
#include "convolute_openblas.h"
#include "convolute_pthread.h"
#include "convolute_multiple.h"
using namespace std;
using namespace std::chrono;
float* read_matrix(int *size, char *name){
/*Reads (square_matrix) data given in the file with above name.*/
/*modifies the size variable such that *size will store the row/column size*/

  ifstream file;
  try{
    file.open(name);
  }
  catch (const ifstream::failure& e){
    cout<<"Exception opening the file "<<name << endl;
    file.close();
    return NULL;
  }

  vector <float> input_vec;
  float val_read;
  while(file>>val_read){
    input_vec.push_back(val_read);//"name" file should contain the matrix in row major order
  }
  int nrows = round(sqrt(input_vec.size()));
  if(nrows*nrows != input_vec.size()){
    cout<<"Error:Matrix in "<<name<< "is not square. Please make sure it is"<<endl;
    file.close();
    return NULL;
  }

  int ncols = nrows;
  if(nrows==0){
		cout<<"Error:"<<name<<" is empty"<<endl;
    file.close();
		return NULL;
	}//exit if file is empty

	float * matrix = new float[nrows*ncols];


	for(int i = 0;i<input_vec.size();i++){
		matrix[i]=input_vec[i];
	}//reading the matrix since input_vec is read in ROW MAJOR ORDER
  file.close();
	*size = nrows;
	return matrix;
}
float*** read_filter_layers(char*name, int num_filters, int filter_width, int filter_depth, float*biases){
/*
Creates filter layers from the file name specfied in char*name with give variables with their names standing for themselves. THe filter is read in row major order.
The last num_filter values are considered biases

filter_layers[i] denotes ith filter
filter_layers[i][j] denotes jth matrix in row major order from ith filter
all i j in 0 base indexing
*/
        int num_biases = num_filters;

        ifstream file;
        try{
          file.open(name);
        }
        catch (const ifstream::failure& e){
          cout<<"Exception opening the file "<<name << endl;
          file.close();
          return NULL;
        }

        float***filter_layers = new float**[num_filters];

        for(int i = 0;i<num_filters;i++){
          filter_layers[i] = new float*[filter_depth];
          for(int j = 0;j<filter_depth;j++)filter_layers[i][j] = new float [filter_width*filter_width];
        }

        float value;

        for(int i = 0;i<num_filters;i++){
          for(int j = 0;j<filter_depth;j++){
            for(int k = 0;k< filter_width*filter_width;k++){
              if(  file>>value ){

                 filter_layers[i][j][k]=value;
              }
               else{

                 cout<<"ERROR While Reading "<<name<<endl;
                 file.close();
                 return NULL;
               }
            }
          }
        }
        for(int i = 0;i<num_biases;i++){
          if(file>>value){
            biases[i] = value;// reading the bias values
          }
          else {
            cout<<"ERROR: While reading for bias values from "<<name<<endl;
            file.close();
            return NULL;
          }
        }
        file.close();
        return filter_layers;

}


int main(int argc, char** argv){
  // To run the input->        ./exe processed_data.txt conv1.txt conv2.txt fc1.txt fc2.txt

  // input_matrix -> conv1 -> pooling -> con
  // auto t1  = std::chrono::high_resolution_clock::now();
   float *input_matrix;
   int input_row_size;

   input_matrix = read_matrix(&input_row_size,argv[1]);


   float** input_layers = new float*[1];
   input_layers[0] = input_matrix;

   float* biases = new float[20];
   float *** filter_layers = read_filter_layers(argv[2],20,5,1,biases);
   float *temp = filter_layers[0][0];// expected filter as  20 X 1 X 25
  //conv1

   float** output_layers = convolute_multiple(input_layers, 28,1,filter_layers,biases,5,20,0,false,false);
  // output_layer   20 X 576


   int num_channels_output_layers = 20;
   for(int i = 0;i<num_channels_output_layers;i++){
     output_layers[i] = maxpool(2,24,2,0,output_layers[i]);
   }

   biases = new float[50];
   filter_layers = read_filter_layers(argv[3],50,5,20,biases);
   output_layers = convolute_multiple(output_layers,12,20,filter_layers,biases,5,50,0,false,false);
    // filter should be as 50 X 20 X 144
    //conv2

   num_channels_output_layers = 50;
   for(int i = 0;i<num_channels_output_layers;i++){
     output_layers[i] = maxpool(2,8,2,0,output_layers[i]);
   }

   biases = new float[500];
   filter_layers = read_filter_layers(argv[4],500,4,50,biases);
   output_layers = convolute_multiple(output_layers,4,50,filter_layers,biases,4,500,0,false,true);

   biases = new float[10];
   filter_layers = read_filter_layers(argv[5],10,1,500,biases);

   output_layers = convolute_multiple(output_layers,1,500,filter_layers,biases,1,10,0,false,false);
   //output_layers 10 X 1 X 1 (for picorial representation) other wise it is just 10 X (1X1)




   float* prob_vector = new float[10];
   for(int i = 0;i<10;i++)prob_vector[i] = output_layers[i][0];
   prob_vector = softmax(prob_vector,10);//softmaxing

    pair<float,int> pairs[10];
    for(int i = 0;i<10;i++){
      pairs[i].first = prob_vector[i];
      pairs[i].second = i;
    }

    sort(pairs,pairs+10,std::greater<>());

   cout<<"Top 5 predicted digits in decreasing of their probabilities "<<endl;
   for(int i =0;i<5;i++)cout<<"Digit "<<pairs[i].second<<"- Probability:"<<pairs[i].first<<endl;

//   auto t2  = std::chrono::high_resolution_clock::now();
//   std::chrono::duration<float> elapsed = t2-t1;
//   cout<<"Time :"<<elapsed.count()<<"\n";

   return 0;
}
