#include<iostream>
#include<vector>
#include<utility>
#include<iterator>
#include <chrono>
using namespace std;
using namespace std::chrono;
float** convoluteByIteration(int kernel_size,int input_size,float **kernel,float** input_matrix){
  //ASSUMES SQUARE MATRICES ONLY
	//assuming the size of a as n*n
	//assuming the size of b as m*m

  float ** output = new float*[input_size-kernel_size+1];

  int output_size = input_size-kernel_size+1;
	for(int i = 0;i<output_size;i++){
    output[i] = new float[output_size];
	}//created output_matrix

	for(int x= 0;x<output_size;x++){
    for(int y = 0;y<output_size;y++){
      output[x][y]=0;
       for(int u = 0;u<kernel_size;u++){
         for(int v = 0 ;v<kernel_size;v++){
             output[x][y]+= (input_matrix[x+u][y+v]*kernel[u][v]);//one to one overlap
         }
       }
    }
  }
  return output;
}

float **convoluteByMatrixMultiplication(int kernel_size,int input_size,float** kernel_matrix,float** input_matrix){
  int temp_nrows = (input_size-kernel_size+1)*(input_size-kernel_size+1);
  int temp_ncols = kernel_size*kernel_size; // temp_matrix is the toeplitz matrix which was uploaded on piazza
  //the number of columns is the number of elements in kernel_vec
  //number of rows is the number of posititions the kernel could take. 1st row being by placing the kernel(after rotation) on the top left corner.
  //Next ones by oving right till possible. Then move down and repeat

  float ** temp_matrix = new float*[temp_nrows];
  for(int i = 0;i<temp_nrows;i++){
    temp_matrix[i] = new float[temp_ncols];
  }


  for(int i = 0;i<temp_nrows;i++){
    int x,y;
    x = i/(input_size-kernel_size+1);
    y = i%(input_size-kernel_size+1);
    for(int j = 0;j<temp_ncols;j++){
      temp_matrix[i][j] = input_matrix[x+j/kernel_size][y+(j%kernel_size)];
    }
  }

  float * kernel_vec = new float[kernel_size*kernel_size];//creating the vector to represent of kernel.
  int kernel_vec_size = temp_ncols;

  for(int i = 0;i<kernel_vec_size;i++){
    kernel_vec[i] = kernel_matrix[i/kernel_size][i%kernel_size];//Read in straight order for one to one overlap
  }

  float ** output = new float*[input_size-kernel_size+1];//output matrix
  int output_size = input_size-kernel_size+1;
	for(int i = 0;i<output_size;i++){
    output[i] = new float[output_size];
	}// output_vec = temp_matrix X kernel_vec;
  //auto t1=std::chrono::high_resolution_clock::now();

  float* output_vec = new float[temp_nrows];
  for(int i = 0;i<temp_nrows;i++){
    output_vec[i]=0;
    for(int j = 0;j<temp_ncols;j++)
      {
        //printf("Value calculated is %f in tid %d\n",output,vec_size);
        output_vec[i] += kernel_vec[j]*temp_matrix[i][j];

      }
     // printf("Value calculated is %f in i=%d\n",output_vec[i],i);
  }
  //auto t2=std::chrono::high_resolution_clock::now();
  //std::chrono::duration <float> lol=t2-t1;
  //cout<<lol.count()<<endl;
  for(int i =0;i<temp_nrows;i++){
    int z = input_size-kernel_size+1;
    output[i/z][i%z]=output_vec[i];
  }//returns the output matrix

  return output;
}


float ** convolution(int kernel_size,int input_size,float** kernel_matrix,float** input_matrix,bool byMultiplication,bool padding,int padding_size=0){
  float ** output;//answer of convolution
  if(padding){

      int new_matrix_size = input_size+2*padding_size;//since padding means adding p rows up and down, p columns to left and right. The length of square increases by 2*padding_size
      float ** new_input_matrix = new float*[new_matrix_size];
      for(int i = 0;i<new_matrix_size;i++)new_input_matrix[i]= new float[new_matrix_size];

      for(int i = 0;i<new_matrix_size;i++){
        for(int j = 0;j<new_matrix_size;j++){
          if(i<padding_size || i>=new_matrix_size-padding_size){
            new_input_matrix[i][j]=0;//making the additionally added rows 0
          }
          else if(j<padding_size || j>=new_matrix_size-padding_size){
            new_input_matrix[i][j]=0;//making the additionally added column element zero
          }
          else new_input_matrix[i][j] = input_matrix[i-padding_size][j-padding_size];

        }
      };


      if(byMultiplication){
         output = convoluteByMatrixMultiplication(kernel_size,new_matrix_size,kernel_matrix,new_input_matrix);
      }
      else{
        output = convoluteByIteration(kernel_size,new_matrix_size,kernel_matrix,new_input_matrix);
      }

  }
  else{
    if(byMultiplication){
       output = convoluteByMatrixMultiplication(kernel_size,input_size,kernel_matrix,input_matrix);
    }
    else{
      output = convoluteByIteration(kernel_size,input_size,kernel_matrix,input_matrix);
    }

  }

  return output;
}
