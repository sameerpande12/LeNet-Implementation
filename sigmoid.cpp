#include <iostream>
#include <math.h>
using namespace std;

double* sigmoid(double* input_vector,int size){
  //returns element by element sigmoid of input_vector as a vector (double*)
  double *output_vector = (double*)malloc(sizeof(double)*size);

  double denominator  = 0.0;

  for(int i = 0;i<size;i++){
    output_vector[i] = 1/(1+expf(-input_vector[i]));
  }

  return output_vector;
}

/*
int main(){

  double  a[] = {2.2,3.1,-.9,1.9};
  double * b = sigmoid(a,4);
  for(int i = 0;i<4;i++)cout<<b[i]<<" "<<endl;

}
*/
