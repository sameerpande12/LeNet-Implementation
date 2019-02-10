#include <iostream>
#include <fstream>
using namespace std;
double**  read_matrix_sized(pair<int, int> size, char *name){
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
	  string temp="start";
	  if(!getline(file, temp)){ cout<<"Empty File "<<name<<endl; return NULL;}
	  file.close();
		try{
	    file.open(name);
	  }
	  catch (const ifstream::failure& e) {
	    cout << "Exception opening/reading file "<<name<<endl;;
	    return NULL;
	  }
	  double **a = new double*[nrows];
	  for(int i = 0; i < nrows; i++)
	      a[i] = new double[ncols];

	 for(int i = 0;i<ncols;i++){
		 for(int j=0;j<nrows;j++){
          file>>a[j][i];
		 }
	 }
	  file.close();
	  return a;
}
void printMatrix(double** answer,int nrows,int ncols){
	for(int i = 0;i<nrows;i++){
		for(int j = 0;j<ncols;j++)cout<<answer[i][j]<<"    ";
		cout<<endl;
	}
}


int main(int argc, char** argv){
  ifstream file;
  file.open(argv[1]);
  pair<int,int>size;
  size.first= atoi(argv[2]);
  size.second = atoi(argv[3]);
  double ** a = read_matrix_sized(size,argv[1]);
  printMatrix(a,size.first,size.second);
  file.close();


}
