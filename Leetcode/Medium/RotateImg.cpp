#include "Hieudz.h"
// You are given an n x n 2D matrix representing an image, rotate the image by 90 degrees (clockwise).

// You have to rotate the image in-place, which means you have to modify the input 2D matrix directly. 
// DO NOT allocate another 2D matrix and do the rotation.
class Solution {
    public:
        void rotate(vector<vector<int>>& matrix) {
            vector<vector<int>> temp(matrix.size(), vector<int>(matrix[0].size()));
            for(int i = 0; i < matrix.size() ; i++){
                for(int j = 0 ;j < matrix[0].size() ; j++){
                    temp[j][matrix.size() - 1 - i] = matrix[i][j];
                }
            }
            for(int i = 0; i < matrix.size() ; i++){
                for(int j = 0 ;j < matrix[0].size() ; j++){
                    matrix[i][j] = temp[i][j];
                }
            }

        }
    };