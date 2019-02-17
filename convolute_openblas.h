#pragma once
using namespace:: std;

float * convolute_openblas(int kernel_size, int input_size,float* kernel_matrix,float* input_matrix,bool toPad, int pad_size);
