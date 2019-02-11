#include <iostream>
using namespace std;

double* maxpool(int filter_size,int matrix_size,int stride,double* A){
// considering only the case when filter_size divides the matrix_size
// output_size-1<=(matrix_size-filter_size)/stride
  int output_size = (matrix_size+stride-1)/stride;

  double * C = new double[output_size*output_size];


  int i = 0;
  for(int i = 0;i<=(output_size-1)*stride;i=i+stride){
   for(int j = 0;j<=(output_size-1)*stride; j=j+stride){

     int x = i;
     int y = j;
     int tempi = i/stride;
     int tempj = j/stride;
     C[tempi*output_size+tempj] = A[x*matrix_size+y];
     int u_limit = filter_size;//u_limit is the maximum permissible value of u so a
     if(x+filter_size-1>matrix_size-1){//occurs when top-left corner of filter is on the input but the top right corner is not
       u_limit = matrix_size - x;
     }

     int v_limit = filter_size;
     if(y+filter_size-1> matrix_size -1){//occurs when top-left corner of filter is in the input but not the bottom left corner
       v_limit = matrix_size -y;
     }

     for(int u = 0;u<u_limit;u++)
       for(int v =0;v<v_limit;v++)
           if(C[tempi*output_size+tempj]<A[(u+x)*matrix_size+v+y])C[tempi*output_size+tempj]=A[(u+x)*matrix_size+v+y];

   }
  }

 return C;
}

double* maxpool(int filter_size,int matrix_size,int stride,int padding_size,double* A){
  int new_matrix_size = matrix_size + 2*padding_size;

  double * B = new double[new_matrix_size*new_matrix_size];


  for(int i = 0;i<new_matrix_size;i++){
    if(i<padding_size || i>=new_matrix_size-padding_size){
      for(int j =0;j<new_matrix_size;j++)B[i*new_matrix_size+j]=0;
    }
    else{
      for(int j=0;j<new_matrix_size;j++){
        if(j<padding_size || j>=new_matrix_size-padding_size)B[i*new_matrix_size+j]=0;
        else B[i*new_matrix_size+j]=A[(i-padding_size)*matrix_size+j-padding_size];
      }

    }
  }

  double * C = maxpool(filter_size,new_matrix_size,stride,B);
  return C;
}



/*
int main(){
  int matrix_size = 10; int filter_size = 3; int stride=2;
  int output_size = (matrix_size-filter_size+stride)/stride;
  double** a = new double*[matrix_size];
  for(int i = 0;i<matrix_size;i++){
    a[i]= new double[matrix_size];
  }
  for(int i = 0;i<matrix_size;i++){
    for(int j = 0;j<matrix_size;j++ ){
      a[i][j]=(i+j)*1.0;
    }

  }
  double **b ;
  b = maxpool(filter_size,matrix_size,stride,a);

  for(int i = 0;i<output_size;i++){
    for(int j = 0;j<output_size;j++)cout<<b[i][j]<<" ";
    cout<<endl;
  }
}
*/
