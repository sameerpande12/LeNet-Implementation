#pragma once
using namespace:: std;

double * convolute_openblas(int kernel_size, int input_size,double* kernel_matrix,double* input_matrix,bool toPad, int pad_size);
