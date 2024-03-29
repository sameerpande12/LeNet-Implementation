
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <chrono>
using namespace std;
using namespace std::chrono;
struct thread_args
 {
    float *vector_, *mat_row;
    float *answer;
    int size,start,end,tid;

};
void* matrix_pthread_helper(void * struct_pointer)
{
  struct thread_args *structure=(struct thread_args *)struct_pointer;
  float *vec=structure->vector_;
  float *mat_row=structure->mat_row;
  int vec_size=structure->size;
  int tid=structure->tid;
  int start=structure->start;
  int end=structure->end;
  float *answer_temp=structure->answer;

  for(int i=0;i<(int)vec_size;++i);


  float output[end-start];
  for(int j=start;j<end;j++)
  {
    answer_temp[j]=0;
    for(int i=0;i<vec_size;++i)
      answer_temp[j]+=mat_row[j*vec_size+ i]*vec[i];
  }

  int a[]={1,3};
  void *x=a;
  return x;


}
float *convolute_pthread(int kernel_size,int input_size,float* kernel_vec,float* input_matrix, int no_threads){
  int temp_nrows = (input_size-kernel_size+1)*(input_size-kernel_size+1);
  int temp_ncols = kernel_size*kernel_size; // temp_matrix is the toeplitz matrix which was uploaded on piazza
  //the number of columns is the number of elements in kernel_vec
  //number of rows is the number of posititions the kernel could take. 1st row being by placing the kernel(after rotation) on the top left corner.
  //Next ones by oving right till possible. Then move down and repeat

  float * temp_matrix=new float[temp_ncols*temp_nrows]; ;

  for(int i = 0;i<temp_nrows;i++){
    int x,y;
    x = i/(input_size-kernel_size+1);
    y = i%(input_size-kernel_size+1);
    for(int j = 0;j<temp_ncols;j++){
      temp_matrix[i*temp_ncols + j] = input_matrix[(x+j/kernel_size)*input_size + y+(j%kernel_size)];
    }
  }



  //output matrix
  int output_size = input_size-kernel_size+1;
  // output_vec = temp_matrix X kernel_vec;
  /*do multiplication here*/

  pthread_t threads[no_threads];
  int rc=0;;
  struct thread_args args[no_threads];
  float* output_vec = new float[temp_nrows];
  auto t1=std::chrono::high_resolution_clock::now();
  for(int i=0;i<no_threads;++i)
  {
    //float *temp_arr[4];

    args[i].vector_=kernel_vec;
    args[i].mat_row=temp_matrix;
    args[i].size=temp_ncols;
    args[i].start=i*temp_nrows/no_threads;
    args[i].end=(i+1)*temp_nrows/no_threads;
    args[i].answer=output_vec;
    args[i].tid=i;
    void* pass_value=&args[i];
    rc=pthread_create(&threads[i], NULL, &matrix_pthread_helper, pass_value);

      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }
  }
  for (int i = 0; i < no_threads; i++)
        pthread_join(threads[i], NULL);
  //auto t2=std::chrono::high_resolution_clock::now();
  //std::chrono::duration <float> ti=t2-t1;
  //cout<<ti.count()<<" ";
  //cout<<"Time taken in multi threading is "<<(t2-t1).count()<<endl;
  //read the output_vec and make output matrix. output of the form float [][]

  return output_vec;
}
float *convolute_pthread(int kernel_size,int input_size,float* kernel_matrix,float* input_matrix,bool padding,int padding_size, int num_threads=2){
  //need to check if number of threads is less the number of rows, otherisw invalid input
  float *output;//assumes padding_size to be non negative integer
  if(padding){
    int new_matrix_size = input_size+2*padding_size;//since padding means adding p rows up and down, p columns to left and right. The length of square increases by 2*padding_size
    float * new_input_matrix =new float[new_matrix_size*new_matrix_size];

    for(int i = 0;i<new_matrix_size;i++){
      for(int j = 0;j<new_matrix_size;j++){
        if(i<padding_size || i>=new_matrix_size-padding_size){
          new_input_matrix[i*new_matrix_size+ j]=0;//making the additionally added rows 0
        }
        else if(j<padding_size || j>=new_matrix_size-padding_size){
          new_input_matrix[i*new_matrix_size+ j]=0;//making the additionally added column element zero
        }
        else new_input_matrix[i*new_matrix_size+ j] = input_matrix[(i-padding_size)*input_size+ j-padding_size];

      }
    }

    output = convolute_pthread(kernel_size,new_matrix_size,kernel_matrix,new_input_matrix, num_threads);
  }
  else{
    output = convolute_pthread(kernel_size,input_size,kernel_matrix,input_matrix, num_threads);
  }
  return output;
}
