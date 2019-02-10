#pragma once
using namespace std;

double** maxpool(int filter_size,int matrix_size,int stride,double** A);
double** maxpool(int filter_size,int matrix_size,int stride,int padding_size,double** A);

double** averagepool(int filter_size,int matrix_size,int stride,double** A);
double** averagepool(int filter_size,int matrix_size,int stride,int padding_size,double** A);
