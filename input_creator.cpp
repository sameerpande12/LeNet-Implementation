#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char** argv){
  //    double temp=static_cast <double> (rand()) / static_cast <double> (RAND_MAX);;



    int nrows,ncols ;

    nrows = atoi(argv[1]);
    ncols = atoi(argv[2]);

    double ** arr= new double* [nrows];
    for(int i = 0;i<nrows;i++){
      arr[i] = new double[ncols];
    }
    double min= -1;
    double max= 1;

    char* name = argv[3];
    ofstream file;
    file.open(name);
    for(int i = 0;i<nrows;i++){
      for(int j =0;j<ncols;j++){
        double temp = static_cast <double> (rand()) / static_cast <double> (RAND_MAX);;
        arr[i][j]= min+(max-min)*temp;
      }
    }
    for(int i = 0;i<ncols;i++){
      for(int j =0;j<nrows;j++){
        file<<arr[j][i]<<endl;
      }
    }
    file.close();
return 0;
}
