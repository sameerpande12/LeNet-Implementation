#pragma once
using namespace:: std;

float ** convolute_multiple(float** input_layers, int input_width, int input_channels,float*** filter_layers, float* biases, int filter_width,int output_channels,int pad_size,bool toPad, bool relu);
