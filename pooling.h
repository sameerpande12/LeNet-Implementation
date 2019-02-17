#pragma once
using namespace std;

float* maxpool(int filter_size,int matrix_size,int stride,float* A);
float* maxpool(int filter_size,int matrix_size,int stride,int padding_size,float* A);

float* averagepool(int filter_size,int matrix_size,int stride,float* A);
float* averagepool(int filter_size,int matrix_size,int stride,int padding_size,float* A);
