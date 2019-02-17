#pragma once
using namespace std;

float * convolute_mkl(int kernel_size, int input_size,float* kernel_matrix,float* input_matrix,bool toPad, int pad_size);
