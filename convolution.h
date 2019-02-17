#pragma once
using namespace:: std;

float** convoluteByIteration(int kernel_size,int input_size,float **kernel,float** input_matrix);
float **convoluteByMatrixMultiplication(int kernel_size,int input_size,float** kernel_matrix,float** input_matrix);
float ** convolution(int kernel_size,int input_size,float** kernel_matrix,float** input_matrix,bool byMultiplication,bool padding,int padding_size=0);
