#pragma once
using namespace:: std;

double** convoluteByIteration(int kernel_size,int input_size,double **kernel,double** input_matrix);
double **convoluteByMatrixMultiplication(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix);
double ** convolution(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix,bool byMultiplication,bool padding,int padding_size=0);
