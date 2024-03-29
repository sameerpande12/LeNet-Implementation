#include<iostream>
#include<vector>
#include<utility>
#include<iterator>
#include <time.h>
#include<chrono>
#include <cblas.h>

using namespace std;
using namespace std::chrono;


float * convolute_openblas(int kernel_size, int input_size,float* kernel_matrix,float* input_matrix,bool toPad, int pad_size){
    if(!toPad)pad_size = 0;
    int Arows = (input_size-kernel_size+1+2*pad_size)*(input_size-kernel_size+1+2*pad_size);
    int Acols = kernel_size*kernel_size; // temp_matrix is the toeplitz matrix which was uploaded on piazza
    //the number of columns is the number of elements in kernel_vec
    //number of rows is the number of posititions the kernel could take. 1st row being by placing the kernel(after rotation) on the top left corner.
    //Next ones by oving right till possible. Then move down and repeat

    float * A = (float*) malloc(Arows*Acols*sizeof(float));
    float * B = (float*) malloc(Acols*sizeof(float));




    for(int i =0;i<Acols;i++){
      int x = i%kernel_size;
      int y = i/kernel_size;

      B[i] = kernel_matrix[kernel_size*y + x]; //computes B matrix for multiplication // kernel_matrix in row major order
    }


   for(int i = 0;i<Arows;i++){
       int x,y;
       x = i/(input_size-kernel_size+1);
       y = i%(input_size-kernel_size+1);
       for(int j = 0;j<Acols;j++){
         int tempy = y+ j%kernel_size;
         int tempx = x + j/kernel_size;

         tempx = tempx - pad_size;
         tempy = tempy - pad_size;
         if( tempx<input_size && tempx>=0 && tempy<input_size && tempy>=0){
                 A[i*Acols + j] = input_matrix[tempx*input_size + tempy];

               }
         else A[i*Acols + j] = 0;
       }
   }

   float * C = (float*) malloc(Arows*sizeof(float));
   int m = Arows;
   int k = Acols;
   int n = 1;
   float alpha = 1.0;
   float beta = 0.0;
    //auto t1  = std::chrono::high_resolution_clock::now();
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
               m, n, k, alpha, A, k, B, n, beta, C, n);
    //auto t2  = std::chrono::high_resolution_clock::now();

  //   std::chrono::duration<float> elapsed = t2-t1;
//   cout<<elapsed.count()<<"\t";
   //cout<<"Matirx (including padding) dimension:"<<input_size+2*pad_size<<endl;
   //cout<<"Kernel dimension:"<<kernel_size<<endl;



   free(A);
   free(B);
   return C;//C represents the output matrix  in row major order
}
