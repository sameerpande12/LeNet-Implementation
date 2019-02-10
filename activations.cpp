#include <iostream>
#include <math.h>
#define tanhmat(x) tanh(x)
#define relu(a) (a)>0?(a):0
using namespace::std;

double* activations( bool flag,  int numrows, int numcols, double *matrix){
	int m=numrows; //number of rows
	int n=numcols; //number of columns
  
	double *output = new double[m*n];


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
  double** a = new double*[4];
  for(int i = 0;i<4;i++){
    a[i]= new double[4];
  }
  for(int i = 0;i<4;i++){
    for(int j = 0;j<4;j++ ){
      a[i][j]=-(i+j)*1.0;
    }
  }
  double **b ;
   activations(false,make_pair(4,4),a);
  cout<<a[0][0]<<" "<<a[0][1]<<endl<<a[1][0]<<" "<<a[1][1]<<endl;
  return 0;
}
*/
