#include <iostream>
#include <math.h>
using namespace std;

float* softmax(float* input_vector,int size){
  //returns matrix of probabilities as per specification of softmax function
  float *output_vector = (float*)malloc(sizeof(float)*size);

  float denominator  = 0.0;
  for(int i = 0;i<size;i++)
    denominator += expf(input_vector[i]);

  for(int i = 0;i<size;i++){
    output_vector[i] = (expf(input_vector[i]))/denominator;
  }

  return output_vector;
}
/*
int main(){

  float  a[] = {2.2,3.1,-.9,1.9};
  float * b = softmax(a,4);
  for(int i = 0;i<4;i++)cout<<b[i]<<" "<<endl;

}*/
