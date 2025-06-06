#include<iostream>
#include <vector>
#include <algorithm>
#include<string>
using namespace std;

void BF(int Arr[][20],int numVertex,char Start,int ValueArray[], int PreviousArray[]);
string BF_Path(int Arr[][20],int numVertex,char Start,char Goal);