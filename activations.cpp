#include <iostream>
#include <math.h>
#define tanhmat(x) tanh(x)
#define relu(a) (a)>0?(a):0
using namespace::std;

float* activations( bool flag,  int numrows, int numcols, float *matrix){
	int m=numrows; //number of rows
	int n=numcols; //number of columns

	float *output = new float[m*n];


	if(flag==true){// if flag = true then compute tanh else relu

		for(int i=0;i<m;++i){
			for(int j=0;j<n;++j){
				output[i*m+j]=tanhmat(matrix[i*m+j]);
			}
		}
	}
	else{
		for(int i=0;i<m;++i){
			for(int j=0;j<n;++j){

				output[i*m+j]=relu(matrix[m*i+j]);
			}
		}
	}
	return output;
}



/*
int main(){
  float** a = new float*[4];
  for(int i = 0;i<4;i++){
    a[i]= new float[4];
  }
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++ ){
      a[i][j]=-(i+j)*1.0;
    }
  }
  float **b ;
   activations(false,make_pair(4,4),a);
  cout<<a[0][0]<<" "<<a[0][1]<<endl<<a[1][0]<<" "<<a[1][1]<<endl;
  return 0;
}
*/
