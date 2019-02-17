#include <iostream>
using namespace std;

float* averagepool(int filter_size,int matrix_size,int stride,float* input_matrix){
// considering only the case when filter_size divides the matrix_size
    int output_size = (matrix_size+stride-1)/stride;
    float * output = new float[output_size*output_size];


    int i = 0;
    for(int i = 0;i<=(output_size-1)*stride;i=i+stride){
     for(int j = 0;j<=(output_size-1)*stride; j=j+stride){

       int x = i;
       int y = j;
       int tempi = i/stride;
       int tempj = j/stride;
       output[tempi*output_size+ tempj] = 0;

       int u_limit = filter_size;
       if(x+filter_size-1>matrix_size-1){
         u_limit = matrix_size - x;
       }

       int v_limit = filter_size;
       if(y+filter_size-1> matrix_size -1){
         v_limit = matrix_size -y;
       }
         for(int u = 0;u<u_limit;u++)
           for(int v =0;v<v_limit;v++)
               output[tempi*output_size+tempj]+=input_matrix[(u+x)*matrix_size+v+y];
            //tempi tempj position in the output matrix


         output[tempi*stride+tempj]/=(u_limit*v_limit);

     }
   }

 return output;
}

float* averagepool(int filter_size,int matrix_size,int stride,int padding_size,float* input_matrix){
  int new_matrix_size = matrix_size + 2*padding_size;
  float * padded_input = new float[new_matrix_size*new_matrix_size];


  for(int i = 0;i<new_matrix_size;i++){
    if(i<padding_size || i>=new_matrix_size-padding_size){
      for(int j =0;j<new_matrix_size;j++)padded_input[i*new_matrix_size+j]=0;
    }
    else{
      for(int j=0;j<new_matrix_size;j++){
        if(j<padding_size || j>=new_matrix_size-padding_size)padded_input[i*new_matrix_size+j]=0;
        else padded_input[i*new_matrix_size+j]=input_matrix[(i-padding_size)*matrix_size+j-padding_size];
      }

    }
  }

  float * output = averagepool(filter_size,new_matrix_size,stride,padded_input);
  return output;
}
/*
int main(){
  int matrix_size = 10; int filter_size = 3; int stride=2;
  int output_size = (matrix_size-filter_size+stride)/stride;
  float** a = new float*[matrix_size];
  for(int i = 0;i<matrix_size;i++){
    a[i]= new float[matrix_size];
  }
  for(int i = 0;i<matrix_size;i++){
    for(int j = 0;j<matrix_size;j++ ){
      a[i][j]=(i+j)*1.0;
    }

  }
  float **b ;
  b = averagepool(filter_size,matrix_size,stride,a);

  for(int i = 0;i<output_size;i++){
    for(int j = 0;j<output_size;j++)cout<<b[i][j]<<" ";
    cout<<endl;
  }
}
*/
