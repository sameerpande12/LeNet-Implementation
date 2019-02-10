#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <vector>
#include "activations.h"
#include "convolution.h"
#include "pooling.h"
#include "sigmoid.h"
#include "softmax.h"
#include "convolute_mkl.h"
#include "convolute_openblas.h"
#include "convolute_pthread.h"
#include "convolute_multiple.h"
using namespace std;

double* read_matrix(int *size, char *name){
/*Reads (square_matrix) data given in the file with above name.*/
/*modifies the size variable such that *size will store the row/column size*/

  ifstream file;
  try{
    file.open(name);
  }
  catch (const ifstream::failure& e){
    cout<<"Exception opening the file "<<name << endl;
    return NULL;
  }

  vector <double> input_vec;
  double val_read;
  while(file>>val_read){
    input_vec.push_back(val_read);//"name" file should contain the matrix in row major order
  }
  int nrows = round(sqrt(input_vec.size()));
  if(nrows*nrows != input_vec.size()){
    cout<<"Error:Matrix in "<<name<< "is not square. Please make sure it is"<<endl;
  }

  int ncols = nrows;
  if(nrows==0){
		cout<<"Error:"<<name<<" is empty"<<endl;
		return NULL;
	}//exit if file is empty

	double * matrix = new double[nrows*ncols];


	for(int i = 0;i<input_vec.size();i++){
		matrix[i]=input_vec[i];
	}//reading the matrix since input_vec is read in ROW MAJOR ORDER
  file.close();
	*size = nrows;
	return matrix;
}
double*** read_filter_layers(char*name, int num_filters, int filter_width, int filter_depth, double*biases){
        int num_biases = num_filters;
        ifstream file;
        try{
          file.open(name);
        }
        catch (const ifstream::failure& e){
          cout<<"Exception opening the file "<<name << endl;
          return NULL;
        }

        double***filter_layers = new double**[num_filters];

        for(int i = 0;i<num_filters;i++){
          filter_layers[i] = new double*[filter_depth];
          for(int j = 0;j<filter_depth;j++)filter_layers[i][j] = new double [filter_width*filter_width];
        }

        double value;
        for(int i = 0;i<num_filters;i++){
          for(int j = 0;j<filter_depth;j++){
            for(int k = 0;k< filter_width*filter_width;k++){
              if(  file>>value ){
                 filter_layers[i][j][k]=value;
              }
               else{
                 cout<<"ERROR While Reading "<<name<<endl;
                 return NULL;
               }
            }
          }
        }
        for(int i = 0;i<num_biases;i++){
          if(file>>value){
            biases[i] = value;
          }
          else {
            cout<<"ERROR: While reading "<<name<<endl;
            return NULL;
          }
        }
        return filter_layers;

}


int main(int argc, char** argv){
  //./exe processed_data.txt conv1.txt conv2.txt fc1.txt fc2.txt

  // input_matrix -> conv1 -> pooling -> con

   double *input_matrix;
   int input_row_size;

   input_matrix = read_matrix(&input_row_size,argv[1]);


   double** input_layers = new double*[1];
   input_layers[0] = input_matrix;

   double* biases = new double[20];
   double *** filter_layers = read_filter_layers(argv[2],1,5,20,biases);
   double *temp = filter_layers[0][0];

   
   double** output_layers = convolute_multiple(input_layers, 28,1,filter_layers,biases,5,20,0,false,false);


   int num_channels_output_layers = 20;
   for(int i = 0;i<num_channels_output_layers;i++){
     output_layers[i] = averagepool(2,24,2,0,output_layers[i]);
   }

   biases = new double[50];
   filter_layers = read_filter_layers(argv[3],50,5,20,biases);
   output_layers = convolute_multiple(output_layers,12,20,filter_layers,biases,5,50,0,false,false);

   num_channels_output_layers = 50;
   for(int i = 0;i<num_channels_output_layers;i++){
     output_layers[i] = averagepool(2,8,2,0,output_layers[i]);
   }

   biases = new double[500];
   filter_layers = read_filter_layers(argv[4],500,4,50,biases);
   output_layers = convolute_multiple(output_layers,4,50,filter_layers,biases,4,500,0,false,false);

   biases = new double[10];
   filter_layers = read_filter_layers(argv[5],10,1,500,biases);
   output_layers = convolute_multiple(output_layers,1,500,filter_layers,biases,1,10,0,false,false);
   //output_layers 10 X 1 X 1 (for picorial representation) other wise it is just 10 X (1X1)

   double* prob_vector = new double[10];
   for(int i = 0;i<10;i++)prob_vector[i] = output_layers[i][0];
   prob_vector = softmax(prob_vector,10);

   for(int i = 0;i<10;i++){
     cout<<i+1<<": "<<prob_vector[i]<<endl;
   }
  return 0;
}
