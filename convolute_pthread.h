#pragma once
using namespace std;

double **convolute_pthread(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix,bool padding,int pad_size,int num_threads = 2);
double **convolute_pthread(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix,int num_threads =2);
