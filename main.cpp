#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>
#include <iterator>
#include <math.h>
#include <vector>
#include "activations.h"
#include "convolution.h"
#include "pooling.h"
#include "sigmoid.h"
#include "softmax.h"
#include "convolute_mkl.h"
#include "convolute_openblas.h"
#include "convolute_pthread.h"

using namespace std;
void printVector(double* vec, int size){
	for(int i = 0;i<size;i++){
		cout<<vec[i]<<endl;
	}
	return;
}

double** read_matrix(int *size, char *name){//reads square matrix from file(in column major order). modifies the *size to store the length of square matrix

	ifstream file;
  try{
    file.open(name);
  }
  catch (const ifstream::failure& e) {
    cout << "Exception opening/reading file "<<name<<endl;;
    return NULL;
  }//checks if file is readable

	vector<double>input_vec;
	if(file.fail()){
		cout<<"Exception opening/reading file "<<name<<endl;
		return NULL;
	}//
	double temp;

	while(file>>temp){
		input_vec.push_back(temp);
	}//store all the non-whitespace elements in the input_vector

	int nrows = round(sqrt(input_vec.size()));
	if(nrows*nrows != input_vec.size()){//checks if matrix is square or not
		cout<<"Matrix is not square"<<endl;
		return NULL;
	}//exit if not square
	int ncols = nrows;
  if(nrows==0){
		cout<<"Error:"<<name<<" is empty"<<endl;
		return NULL;
	}//exit if file is empty

	double ** matrix = new double*[nrows];
	for(int i = 0;i<nrows;i++){
		matrix[i]= new double[ncols];
	}

	for(int i = 0;i<input_vec.size();i++){
		matrix[i%nrows][i/nrows]=input_vec[i];
	}//accounting for column major order
  file.close();
	*size = nrows;
	return matrix;
}

double* read_vector(int *size, char *name){//reads vector from given file with name, and modifies *size to store the value of actual size

	ifstream file;
  try{
 	 file.open(name);
  }
  catch (const ifstream::failure& e) {
 	 cout << "Exception opening/reading file "<<name<<endl;;
 	 return NULL;
 }

  vector<double>input_vec;
  if(file.fail()){
 	 cout<<"Exception opening/reading file "<<name<<endl;
 	 return NULL;
  }
  double temp;

  while(file>>temp){
 	 input_vec.push_back(temp);
 }//read non white space numbers

  int nrows = input_vec.size();
  *size = nrows;
   if(nrows==0){
		 cout<<"Emtpy file: "<<name<< endl;
		 return NULL;
	 }


	double* answer = new double[*size];
	for(int i = 0;i<*size;i++)answer[i]=input_vec[i];

  file.close();
  return answer;
}
double**  read_matrix_sized(pair<int, int> size, char *name){//reads matrix assuming given size (size.first X size.second), from file with given name.
	//returns NULL if unable to read or empty file. else returns the matrix (double **)
    ifstream file;
	  int nrows=size.first;
	  int ncols=size.second;
	  try{
	    file.open(name);
	  }
	  catch (const ifstream::failure& e) {
	    cout << "Exception opening/reading file "<<name<<endl;;
	    return NULL;
	  }
	  vector<double> b;
		double temp;
		while(file>>temp){
			b.push_back(temp);
		}

		if(nrows*ncols < b.size()){
			cout<<"Error: Number of elements in the input file are greater than "<<ncols*nrows<<endl;
			return NULL;
		}
		else if(nrows*ncols>b.size()){
			cout<<"Error: Number of elements in the input file are less than  "<<ncols*nrows<<endl;
			return NULL;
		}

	  double **a = new double*[nrows];
	  for(int i = 0; i < nrows; i++)
	      a[i] = new double[ncols];

    int count = 0;
		 for(int i = 0;i<ncols;i++){
			 for(int j=0;j<nrows;j++){
	         a[j][i]= b[count];
					 count++;
			 }
		 }//stores the elements of vector into the matrix

	  file.close();
	  return a;
}

void printMatrix(double** answer,int nrows,int ncols){//prints matrix with each line containing one row
	for(int i = 0;i<nrows;i++){
		for(int j = 0;j<ncols;j++)cout<<answer[i][j]<<"    ";
		cout<<endl;
	}
}
void printMatrix(double* output, int nrows, int ncols){
	//output is present as an array / vector in row major order
	for(int i = 0;i<ncols*nrows;i++){
		cout<<output[i]<<" ";
		if(i%ncols == ncols-1)cout<<endl;
	}
}

int main(int argc, char **argv){

	if(argc>7){
			cout<<"Error: Too many arguments passed. No function takes in so many arguments"<<endl;
			return 1;
	}

  if(argc<=1){
    cout<<"Error: Very less arguments passed. No function takes this less arguments."<<endl;
    return 1;
  }
  // ensures that argc>=2
	ifstream file1, file2;
	bool convolute_flagvalue=false;//
	bool activate_flagvalue=false;
	bool pooling_flagvalue=false;
	//cout<<strncmp("convolute", argv[1],9)<<" it is why so "<<argv[1]<<endl;
	if (strcmp("convolute_m",argv[1])==0 && (argc==5 || argc ==7) ){
	  //convolute_m padding pad_size input.txt kernel.txt openblas argc->7
	  //convolute_m padding pad_size input.txt kernel.txt mkl
	  //convolute_m  input.txt kernel.txt openblas ->argc 5
	  //convolute_m  input.txt kernel.txt mkl
	  int offset = 0;
	  int pad_size = 0;
	  string help = "Please ensure your format is correct\nconvolute_m padding pad_size input.txt kernel.txt {openblas/mkl}\nconvolute_m input.txt kernel.txt {openblas/mkl/pthread}\n";
	  if(argc<5){
	    cout<<"Less number of parameters passed for convolute_m\n";
	    cout<<help;
	    return 1;
	  }
	  else if(argc>7){
	    cout<<"Extra parameters passed for convolute_m"<<endl;
	    cout<<help;
	    return 1;
	  }
	  else if(argc == 6){
	    cout<<"Please check your format"<<endl;
	    cout<<help;
	    return 1;
	  }
	  else if(argc==7 && strcmp("padding",argv[2])!=0){
	    cout<<"Did you mean "<<"padding"<<" instead of"<<argv[2]<<endl;
	    return 1;
	  }
	  else if(argc==7){
	    pad_size = atoi(argv[3]);
	    if(pad_size<0){
	      cout<<"Padding size cannot be negative"<<endl;
	      return 1;
	    }
	    offset = 2;
	  }

    if(strcmp(argv[4+offset],"pthread")==0){
			double **input, **kernel;
			int input_size,kernel_size;

			input=read_matrix(&input_size, argv[2+offset]);
			kernel=read_matrix(&kernel_size, argv[3+offset]);
			if(input==NULL||kernel==NULL){
				return 1;
			}

			if(kernel_size>input_size){
				cout<<"Kernel_size must be smaller than input. Try flipping kernel and input files with appropriate padding"<<endl;
				return 1;
			}

			double **output;
			if(offset==2)output=convolute_pthread(kernel_size, input_size, kernel, input,true,pad_size,2);
			else output = convolute_pthread(kernel_size,input_size,kernel, input,2);

			int output_size = input_size-kernel_size+1+2*pad_size;//since pad_size was initialized to be 0 and modified only if pad_size was an argument and was valid
			//printMatrix(output,output_size,output_size);

		}

		else{
			double * input, *kernel;
		  int input_size; int kernel_size;

		  input = read_vector(&input_size,argv[2+offset]);
		  kernel = read_vector(&kernel_size,argv[3+offset]);

		  if(input==NULL || kernel ==NULL){
		    return 1;
		  }
		  if(kernel_size> input_size){
		    cout<<"Kernel size ("<<kernel_size<<") must be smaller than input size ("<<input_size<<")"<<endl;
		    return 1;
		  }

		  if( round(sqrt(input_size))*round(sqrt(input_size))!= input_size){
		    cout<<argv[2+offset]<<" is not a square matrix"<<endl;
		    return 1;
		  }
		  input_size = round(sqrt(input_size));

		  bool is_mkl,is_openblas,is_pthread;
			is_mkl = is_openblas = is_pthread = false;
		  if(strcmp(argv[4+offset],"mkl")==0){
		    is_mkl = true;
		  }
		  else if(strcmp(argv[4+offset],"openblas")==0){
		    is_openblas = true;
		  }

		  else {
		    cout<<"Did you mean {mkl/openblas/pthread} instead of "<<argv[4+offset]<<endl;
		    cout<<help;
		    return 1;
		  }

		  if( round(sqrt(kernel_size))*round(sqrt(kernel_size))!= kernel_size){
		    cout<<argv[3+offset]<<" is not a square matrix"<<endl;
		      return 1;
		  }
		  kernel_size = round(sqrt(kernel_size));

		  double * output;
		  if(is_mkl)output = convolute_mkl(kernel_size,input_size,kernel,input,true,pad_size);
		  else if(is_openblas) output = convolute_openblas(kernel_size,input_size,kernel,input,true,pad_size);


		  int output_size = input_size - kernel_size + 1 + 2*pad_size;

		  //printMatrix(output,output_size,output_size);
   }
	}


  //argv[1] must exist since argc >= 2
	else if(strncmp("convolute", argv[1],9)==0){//checks if first 9 letters of arg1 are convolute

		    /* instruction format
		      convolute_{f} padding pad_size input.txt kernel.txt -> argc  = 6
		      convolute_{f} input kernel -> argc = 4
		    */

				if(strlen(argv[1])>11){
							cout<<"No such function. Did you mean convolute_m (for matrix multiplication method) or convolute_i (for iterive method)"<<endl;
							return 1;
				}//ensures that length of argv[1] is less than 12

		    if(strlen(argv[1])<=10){
		        cout<<"No such function. Did you mean convolute_m (for matrix multiplication method) or convolute_i (for iterive method)"<<endl;
		        return 1;
		    }//ensures length of argv[1] equal to 11

		    if(argv[1][9]!='_'){
		      cout<<"No such function. Did you mean convolute_m (for matrix multiplication method) or convolute_i (for iterive method)"<<endl;
		      return 1;
		    }//ensures that the 10th character is _


				if(argv[1][10]=='m')// ensures correct flag value is present 'm' or 'i'
					convolute_flagvalue=true;
				else if(argv[1][10]!='i'){
					cout<<"No such function. Flag_value can only be m or i"<<endl<<argv[1][10]<<" is not a legal flag" ;
					return 1;
				}

		    if(argc<4){
		      cout<<"Less arguments passed. Make sure you have passed name of input.txt file and kernel.txt file "<<endl;
		      return 1;
		    }//ensures argc>=4

		    if(argc>6){
		      cout<<"Invalid arguments. Following cli are allowed for convolution. {f}-> m/i"<<endl;
		      cout<<"convolute_{f} padding pad_size input.txt kernel.txt"<<endl;
		      cout<<"convolute_{f} {input_filename} {kernel_filename}"<<endl;
		      return 1;
		    }

		    if(argc==5){
		      cout<<"Invalid arguments. following cli are allowed for convolution. {f}-> m/i"<<endl;
		      cout<<"convolute_{f} padding pad_size input.txt kernel.txt"<<endl;
		      cout<<"convolute_{f} {input_filename} {kernel_filename}"<<endl;
		      return 1;
		    }
		    //ensures argc =4 or argc =6
				if(strcmp("padding",argv[2])==0){

					if(argc==4){
		        cout<<"Less arguments for convolution with padding. Ensure that your format is correct. {f}-> m/i"<<endl;
		        cout<<"convolute_{f} padding pad_size {input_filename} {kernel_filename}"<<endl;
		        return 1;
		      }//ensures that argc==6 in this case

					int pad_size=atoi(argv[3]);
		      if(pad_size<0){
		        cout<<"Padding_size cannot be negative. Please ensure that you enter a positive integer."<<endl;
		        return 1;
		      }

					double **input, **kernel;
					int input_size, kernel_size;

					input=read_matrix(&input_size, argv[4]);
					kernel=read_matrix(&kernel_size, argv[5]);
					if(input==NULL||kernel==NULL){//error messages printed in read_matrix
		        return 1;
					}

		      if(kernel_size>input_size){
		        cout<<"kernel must be smaller than input. Try flipping kernel and input files with appropriate padding"<<endl;
		        return 1;
		      }

					double **answer=convolution(kernel_size, input_size, kernel, input, convolute_flagvalue,true, pad_size);
					int output_size = input_size-kernel_size+2*pad_size+1;
					//printMatrix(answer,output_size,output_size);//prints the matrix on terminal

				}
				else{
					if(argc>4){

						cout<<"Error: Only expected input matrix and kernel, extra argument(s) passed for convolution without padding.\nPlease ensure your format is correct. {f}=m/i"<<endl;
						cout<<"convolute_{f} {input_filename} {kernel_filename}"<<endl;
						return 1;
					}
					else if(argc==3){

						cout<<"Error: Two matrices expected, less argument(s) passed for convolution without padding.\nPlease ensure your format is correct. {f}=m/i"<<endl;
		        cout<<"convolute_{f} {input_filename} {kernel_filename}"<<endl;
						return 1;
					}

					double **input, **kernel;
					int input_size,kernel_size;

					input=read_matrix(&input_size, argv[2]);
					kernel=read_matrix(&kernel_size, argv[3]);
					if(input==NULL||kernel==NULL){
						return 1;
					}

		      if(kernel_size>input_size){
		        cout<<"Kernel_size must be smaller than input. Try flipping kernel and input files with appropriate padding"<<endl;
		        return 1;
		      }

					double **answer=convolution(kernel_size, input_size, kernel, input, convolute_flagvalue,false);
					int output_size = input_size-kernel_size+1;
					//printMatrix(answer,output_size,output_size);


				}
	}
	else if(strcmp("activate", argv[1])==0){
		    /*
		     activate relu/tanh input_matrix.txt input_numrows input_numcols
		    */
				if(argc>6){
						cout<<"Error: Extra argument(s) passed for activation."<<endl;
						cout<<"Please check your format:"<<endl;
						cout<<"activate {relu/tanh} {input_filename} {no. of rows in input} {no. of rows in output}"<<endl;
						return 1;
					}
				else if(argc<=5){
						cout<<"Error: Less argument(s) passed activation."<<endl;
						cout<<"Please check your format:"<<endl;
						cout<<"activate {relu/tanh} {input_filename} {no. of rows in input} {no. of rows in output}"<<endl;

						return 1;
					}
        // if relu then activate_flagvalue = true else false
				 //ensured argc==6
				 if(strcmp(argv[2],"relu")==0){
		 			activate_flagvalue=false;
		 		 }
				 else if(strcmp(argv[2],"tanh")==0){
		 			activate_flagvalue=true;
		 		 }
		 		 else{
		 			cout<<"Error: Expected relu/tanh, but "<<argv[2]<< " was found "<<endl;
		 			return 1;
		 		 }


				 double **input, **kernel;
		 			pair<int, int> size=make_pair(atoi(argv[4]), atoi(argv[5]));
		 			//cout<<"done"<<endl;
		      bool wrong_dimension = false;
		 			if(size.first<=0){
						cout<<"Error: The number of rows in matrix must be positive integers."<<endl;
		        wrong_dimension = true;
					}
					if(size.second<=0){
						cout<<"Error: The number of columns in the matrix must be positive integer."<<endl;
						wrong_dimension = true;
					}
					if(wrong_dimension)return 1;


		 			input=read_matrix_sized(size, argv[3]);

		 			if(input==NULL){
		 				return 1;
		 			}
		     double ** answer ;
		 	   answer = activations(activate_flagvalue, size, input);
				 //printMatrix(answer,size.first,size.second);

	}
	else if(strcmp(argv[1], "sigmoid")==0){
		// sigmoid input.txt
				if(argc>3){
					cout<<"Error: Extra arguments passed for sigmoid. Please check your format"<<endl;
					cout<<"sigmoid {input_filename}"<<endl;
					return 1;
				}
				if(argc==2){
					cout<<"Error: Less arguments for sigmoid. Please check your format "<<endl;
					cout<<"sigmoid {input_filename}"<<endl;
					return 1;
				}
				double *vector_input, *answer;
				int size=0;
				vector_input=read_vector(&size, argv[2]);



				answer=sigmoid(vector_input, size);

				//printVector(answer,size);

	}
	else if(strcmp(argv[1], "softmax")==0){

				if(argc>3){
					cout<<"Error: Extra arguments passed for softmax. Please check your format"<<endl;
					cout<<"softmax {input_filename}"<<endl;

					return 1;
				}
				if(argc==2){

					cout<<"Error: Less arguments for softmax. Please check your format "<<endl;
					cout<<"softmax {input_filename}"<<endl;
					return 1;
				}
				double *vector_input, *answer;
				int size=0;
				vector_input=read_vector(&size, argv[2]);
				answer=softmax(vector_input, size);
				//printVector(answer,size);

	}
	else if( (strcmp(argv[1],"maxpool")==0) || (strcmp(argv[1],"averagepool")==0) ){
   /*
	  maxpool padding pd_size input.txt filter_size stride argc->7
		maxpool input.txt filter_size stride  argc->5
	 */
	 if(argc>7){
		 cout<<"Error: Extra parameters passed for pooling. Please check your format:"<<endl;
		 cout<<"{maxpool/averagepool} padding {pad_size} {input_filename} {filter_size} {stride}"<<endl;
		 cout<<"{maxpool/averagepool} {input_filename} {filter_size} {stride}"<<endl;
		 return 1;
	 }
	 else if(argc<5){
		 cout<<"Error: Less parameters passed for pooling. Please check your format:"<<endl;
		 cout<<"{maxpool/averagepool} padding {pad_size} {input_filename} {filter_size} {stride}"<<endl;
		 cout<<"{maxpool/averagepool} {input_filename} {filter_size} {stride}"<<endl;

		 return 1;
	 }
	 else if(argc==6 & strcmp(argv[2],"padding")==0) {
     cout<<"Errr: One parameter missing for pooling with padding. Please check your format"<<endl;
		 cout<<"{maxpool/averagepool} padding {pad_size} {input_filename} {filter_size} {stride}"<<endl;
		 return 1;
	 }
	 else if(argc==6){
		 cout<<"Do you mean "<<argv[1]<<"  with padding"<<endl;
		 cout<<"Error: Invalid function call. Incorrect parameters passed. Please check your format:"<<endl;
		 cout<<"{maxpool/averagepool} padding {pad_size} {input_filename} {filter_size} {stride}"<<endl;
		 cout<<"{maxpool/averagepool} {input_filename} {filter_size} {stride}"<<endl;
		 return 1;
	 }

	 else if(argc==5){
		 /* maxpool input.txt filter_size stride */
		 /* averagepool input.txt filter_size stride */
		 double ** input;
		 int size;
		 input = read_matrix(&size,argv[2]);

		 if(input == NULL){
       cout<<"Error:Invalid matrix. Cannot read/open file or the matrix is empty"<<argv[4]<<endl;
			 return 1;
		 }
		 int filter_size = atoi(argv[3]);
		 int stride = atoi(argv[4]);

		 if(filter_size<=0){
			 cout<<"Error: Filter size cannot be negative or zero. Please enter a positive filter_size"<<endl;
			 return 1;
		 }
		 if(stride<=0){
			 cout<<"Error: Stride cannot be negative or zero. Please enter a positive stride"<<endl;
		 }

		 			if(size<filter_size){
		 				cout<<"Error: size of input_matrix should not be less than filter size"<<endl;
		 				return 1;
		 			}

		 double** answer;
     bool pool_flag = false;
		 if(strcmp(argv[1],"maxpool")==0)pool_flag = true;

		 if(pool_flag){
			 answer = maxpool(filter_size,size,stride,input);
		 }
		 else{
       answer = averagepool(filter_size,size,stride,input);
		 }
		 int output_size = (size+stride-1)/stride;
		 //printMatrix(answer,output_size,output_size);

	 }
	 else if(argc==7){
      /* maxpool padding padding_size input.txt filter_size stride
			   averagepool padding padding_size input.txt filter_size stride*/

			if(strcmp(argv[2],"padding")!=0){
				cout<<"Error. Do you mean "<<argv[1]<<" padding ..."<<endl;
				return 1;
			}
			int padding_size = atoi(argv[3]);
			int filter_size = atoi(argv[5]);
			int stride = atoi(argv[6]);
			if(padding_size<0){
				cout<<"Error: Padding size cannot be negative"<<endl;
				return 1;
			}
			if(filter_size<=0){
				cout<<"Error: Filter size cannot be negative or zero. Please enter a positive filter_size"<<endl;
        return 1;
			}
			if(stride<=0){
				 cout<<"Error: Stride cannot be negative or zero. Please enter a positive stride"<<endl;
				return 1;
			}


			double ** input;
			int size;
      input = read_matrix(&size,argv[4]);//size stores the size of input_matrix

			if(size<filter_size){
				cout<<"Error: size of input_matrix should not be less than filter size"<<endl;
				return 1;
			}
			if(input==NULL){

				return 1;
			}

			double** answer;
      bool pool_flag = false;
 		  if(strcmp(argv[1],"maxpool")==0)pool_flag = true;

     if(pool_flag){
			 answer = maxpool(filter_size,size,stride,padding_size,input);
		 }
		 else{
      answer = averagepool(filter_size,size,stride,padding_size,input);
		 }
		 int output_size = (size+stride+2*padding_size-1)/stride;//

		 //printMatrix(answer,output_size,output_size);
	 }


	}
	else{
		cout<<"Error: Please check your function name. No match found"<<endl;
		return 1;
	}

  return 0;
}
