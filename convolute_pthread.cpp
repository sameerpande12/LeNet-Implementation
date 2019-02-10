
#include <iostream>
#include <vector>
#include <utility>
#include <iterator>
#include <chrono>
using namespace std;
using namespace std::chrono;
struct thread_args
 {
    double *vector_, **mat_row;
    double *answer;
    int size,start,end,tid;

};
void* matrix_pthread_helper(void * struct_pointer)
{
  struct thread_args *structure=(struct thread_args *)struct_pointer;
  double *vec=structure->vector_;
  double **mat_row=structure->mat_row;
  int vec_size=structure->size;
  int tid=structure->tid;
  int start=structure->start;
  int end=structure->end;
  double *answer_temp=structure->answer;

  for(int i=0;i<(int)vec_size;++i);


  double output[end-start];
  for(int j=start;j<end;j++)
  {
    answer_temp[j]=0;
    for(int i=0;i<vec_size;++i)
      answer_temp[j]+=mat_row[j][i]*vec[i];
  }

  int a[]={1,3};
  void *x=a;
  return x;


}
double **convolute_pthread(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix, int no_threads){
  int temp_nrows = (input_size-kernel_size+1)*(input_size-kernel_size+1);
  int temp_ncols = kernel_size*kernel_size; // temp_matrix is the toeplitz matrix which was uploaded on piazza
  //the number of columns is the number of elements in kernel_vec
  //number of rows is the number of posititions the kernel could take. 1st row being by placing the kernel(after rotation) on the top left corner.
  //Next ones by oving right till possible. Then move down and repeat

  double ** temp_matrix = new double*[temp_nrows];
  for(int i = 0;i<temp_nrows;i++){
    temp_matrix[i] = new double[temp_ncols];
  }


  for(int i = 0;i<temp_nrows;i++){
    int x,y;
    x = i/(input_size-kernel_size+1);
    y = i%(input_size-kernel_size+1);
    for(int j = 0;j<temp_ncols;j++){
      temp_matrix[i][j] = input_matrix[x+j/kernel_size][y+(j%kernel_size)];
    }
  }

  double * kernel_vec = new double[kernel_size*kernel_size];//creating the vector to represent of kernel.
  int kernel_vec_size = temp_ncols;

  for(int i = 0;i<kernel_vec_size;i++){
    kernel_vec[i] = kernel_matrix[i/kernel_size][i%kernel_size];//Reading in straight order unlike assignment 1
  }

  double ** output = new double*[input_size-kernel_size+1];//output matrix
  int output_size = input_size-kernel_size+1;
  for(int i = 0;i<output_size;i++){
    output[i] = new double[output_size];
  }// output_vec = temp_matrix X kernel_vec;
  /*do multiplication here*/

  pthread_t threads[no_threads];
  int rc=0;;
  struct thread_args args[no_threads];
  double* output_vec = new double[temp_nrows];
  auto t1=std::chrono::high_resolution_clock::now();
  for(int i=0;i<no_threads;++i)
  {
    //double *temp_arr[4];

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
  //std::chrono::duration <double> ti=t2-t1;
  //cout<<ti.count()<<" ";
  //cout<<"Time taken in multi threading is "<<(t2-t1).count()<<endl;
  for(int i =0;i<temp_nrows;i++){
    int z = input_size-kernel_size+1;
    output[i/z][i%z]=output_vec[i];
  }//read the output_vec and make output matrix. output of the form double [][]

  return output;
}
double **convolute_pthread(int kernel_size,int input_size,double** kernel_matrix,double** input_matrix,bool padding,int padding_size, int num_threads=2){
  //need to check if number of threads is less the number of rows, otherisw invalid input
  double **output;//assumes padding_size to be non negative integer
  if(padding){
    int new_matrix_size = input_size+2*padding_size;//since padding means adding p rows up and down, p columns to left and right. The length of square increases by 2*padding_size
    double ** new_input_matrix = new double*[new_matrix_size];
    for(int i = 0;i<new_matrix_size;i++)new_input_matrix[i]= new double[new_matrix_size];

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
    }

    output = convolute_pthread(kernel_size,new_matrix_size,kernel_matrix,new_input_matrix, num_threads);
  }
  else{
    output = convolute_pthread(kernel_size,input_size,kernel_matrix,input_matrix, num_threads);
  }
  return output;
}
