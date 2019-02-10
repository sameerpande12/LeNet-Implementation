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
    input_vec.push_back(temp);//"name" file should contain the matrix in row major order
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
double*** read_filter_layers(char*name, int num_filters, int filter_width, int filter_depth, double*bias){
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
          for(int j = 0;j<filter_depth;j++)filter_layer[i][j] = new double [filter_width*filter_width];
        }

        double value;
        for(int i = 0;i<num_filters;i++){
          for(int j = 0;j<filter_depth;j++){
            for(int k = 0;k< filter_width*filter_width;k++){
              if(  file>>value ){
                 filter_layers[i][j][k]=value;
              }
               else{
                 cout<<"ERROR While Reading"<<endl;
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

}


int main(int argc, char** argv){
  //./exe processed_data.txt

  // input_matrix -> conv1 -> pooling -> con
   double *input_matrix;
   int input_row_size;
   input_matrix = read_matrix(&input_row_size,argv[1]);

   double** input_layers = new double*[1];
   input_layers[0] = input_matrix;

    double* biases = new double[20];  


  return 0;
}
