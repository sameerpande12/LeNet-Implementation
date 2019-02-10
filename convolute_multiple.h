#pragma once
using namespace:: std;

double ** convolute_multiple(double** input_layers, int input_width, int input_channels,double*** filter_layers, double* biases, int filter_width,int output_channels,int pad_size,bool toPad, bool relu);
