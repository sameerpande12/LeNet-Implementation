#include <iostream>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <vector>
#include "convolute_mkl.h"
#include "convolute_openblas.h"
#include "convolute_pthread.h"
#include "activations.h"
using namespace std;

float ** convolute_multiple(float** input_layers, int input_width, int input_channels,float*** filter_layers, float* biases, int filter_width,int output_channels,int pad_size,bool toPad, bool relu){
//input_layer consists of all input_weights stated in row major order one after other
//input_width is the width/length of each layer of input
//input_channels is the number of layers in input_layers and the number of of layers in a individual filter out of num_filters filters
//filter width is the width/length of each layer of input
  if(!toPad)pad_size = 0;

  int output_width = (input_width - filter_width + 2*pad_size+1);


  int num_weights_input = input_width*input_width;
  int num_weights_filter = filter_width*filter_width;

  float** output_layers = new float*[output_channels];

  for(int j = 0; j < output_channels;j++ ){
    //since each filter will produce output_channel hence j stands for the index of the filter to be applied

    float** tempeval = new float*[input_channels];

    //input_layers[0] is the first 2d matrix of input_layers
    for(int i = 0 ;i < input_channels;i++){
      float *temp = filter_layers[j][i];
      tempeval[i] = convolute_pthread(filter_width, input_width,filter_layers[j][i],input_layers[i],toPad,pad_size);

    }

    int temp_size = (input_width - filter_width + 2*pad_size+1)*(input_width - filter_width + 2*pad_size+1);
    float* templayer = new float [temp_size];
    //flattening to a single layer
    for(int i = 0 ; i < temp_size;i++){
      templayer[i] = biases[j];

      for(int k = 0;k < input_channels;k++){

        templayer[i]+= tempeval[k][i];
      }

      if(relu){
        if(templayer[i]<0) templayer[i]=0;
      }
    }

   output_layers[j] = templayer;

  }
  return output_layers;
}
