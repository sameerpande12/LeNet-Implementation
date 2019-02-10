#include <iostream>
#include <math.h>
#define tanhmat(x) tanh(x)
#define relu(a) (a)>0?(a):0
using namespace::std;

double** activations( bool flag,  pair<const int, const int> rc_matrix, double **matrix){
	int m=rc_matrix.first; //number of rows
	int n=rc_matrix.second; //number of columns

	double **output = new double*[m];
	for(int i = 0;i<m;i++){
		output[i]= new double[n];
	}//creating a matrix

	if(flag==true){// if flag = true then compute tanh else relu

		for(int i=0;i<m;++i){
			for(int j=0;j<n;++j){
				output[i][j]=tanhmat(matrix[i][j]);
			}
		}
	}
	else{
		for(int i=0;i<m;++i){
			for(int j=0;j<n;++j){
				output[i][j]=relu(matrix[i][j]);
			}
		}
	}
	return output;
}


double* activations( bool flag,  int numrows, int numcols, double *matrix){
	int m=numrows; //number of rows
	int n=numcols; //number of columns

	double * output = new double[m*n];//creating a matrix

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
				output[i*m+j]=relu(matrix[i*m+j]);
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
