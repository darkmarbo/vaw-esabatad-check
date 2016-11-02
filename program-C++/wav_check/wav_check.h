#ifndef _WAVCHECK_H
#define _WAVCHECK_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <math.h>
#include <vector>
#include <complex> 
#include <bitset> 
using namespace std;



int ReadFile(const char *wavefile, short* allbuf, int bias, int halfWindow);
int ReadFileLength(const char* wavefile, int* sampleRate);

float findLocalMaximum(float* score, int length);
float findLocalMiximum(float* score, int length);

int check_n(const char *file1, const char *file2);
int check_eq_sub(const short *data, int size);
int check_eq(const char *file1, int chk_len, int del_time);
int check_jiefu(const char *file1);


#endif  
