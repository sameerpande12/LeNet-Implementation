#pragma once
using namespace std;

float *convolute_pthread(int kernel_size,int input_size,float* kernel_matrix,float* input_matrix,bool padding,int pad_size,int num_threads = 2);
float *convolute_pthread(int kernel_size,int input_size,float* kernel_matrix,float* input_matrix,int num_threads =2);
