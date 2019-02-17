#include <iostream>
#include <math.h>
using namespace std;

float* sigmoid(float* input_vector,int size){
  //returns element by element sigmoid of input_vector as a vector (float*)
  float *output_vector = (float*)malloc(sizeof(float)*size);

  float denominator  = 0.0;

  for(int i = 0;i<size;i++){
    output_vector[i] = 1/(1+expf(-input_vector[i]));
  }

  return output_vector;
}

/*
int main(){

  float  a[] = {2.2,3.1,-.9,1.9};
  float * b = sigmoid(a,4);
  for(int i = 0;i<4;i++)cout<<b[i]<<" "<<endl;

}
*/
