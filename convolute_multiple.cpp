#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <vector>
#include "convolute_mkl.h"
#include "convolute_openblas.h"
#include "convolute_pthread.h"
using namespace std;

double ** convolute_multiple(double** input_layers, int input_width, int input_channels,double*** filter_layers, double* biases,int num_filters, int filter_width,int output_channels,int pad_size,bool toPad, bool relu){
//input_layer consists of all input_weights stated in row major order one after other
//input_width is the width/length of each layer of input
//input_channels is the number of layers in input_layers and the number of of layers in a individual filter out of num_filters filters
//filter width is the width/length of each layer of input
  if(!toPad)pad_size = 0;

  int output_width = (input_width - filter_width + 2*pad_size+1);
  int output_depth = output_channels;

  int num_weights_input = input_width*input_width;
  int num_weights_filter = filter_width*filter_width;

  double** output_layers = new double*[output_channels];

  for(int j = 0; j < output_channels;j++ ){
    //since each filter will produce output_channel hence j stands for the index of the filter to be applied

    double** tempeval = new double*[input_channels];
    //input_layers[0] is the first 2d matrix of input_layers
    for(int i = 0 ;i < numlayers;i++){
      tempeval[i] = convolute_mkl(filter_width, input_width,filter_layers[j][i],input_layers[i],toPad,pad_size);
      if(relu){
         int temp_numrows = input_width - filter_width + 2*pad_size + 1
         tempeval[i] = activations (false, temp_numrows, temp_numcols, tempeval[i] );
      }
    }
    int temp_size = (input_width - filter_width + 2*pad_size+1)*(input_width - filter_width + 2*pad_size+1);
    double* templayer;
    //flattening to a single layer
    for(int i = 0 ; i < temp_size;i++){
      templayer[i] = biases[j];
      for(int k = 0;k < input_channels;k++){
        templayer[i]+= tempeval[k][i];
      }
    }

   output_layers[j] = tempout;
  }
  return output_layers;
}
