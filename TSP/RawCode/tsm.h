#include<iostream>
#include <vector>
#include <algorithm>
#include<string>
using namespace std;

int firstMin(int Arr[20][20], int numVertex, int a);
int secondMin(int Arr[20][20], int numVertex, int a);
void TSPRecursive(int Arr[20][20], int bound, int total_cost,int level, int path[],int numVertex);
string Traveling(int Arr[20][20],int numVertex,char Start);