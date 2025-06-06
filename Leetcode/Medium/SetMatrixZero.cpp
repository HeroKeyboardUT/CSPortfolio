#include "Hieudz.h"

class Solution {
    public:
        void setZeroes(vector<vector<int>>& matrix) {
            vector<pair<int, int>> zeroPos;
            for (int i = 0; i < matrix.size(); ++i) {
                for (int j = 0; j < matrix[0].size(); ++j) {
                    if (matrix[i][j] == 0) {
                        zeroPos.push_back({i, j});
                    }
                }
            }
            for (const auto& pos : zeroPos) {
                int row = pos.first;
                int col = pos.second;
                for (int i = 0; i < matrix.size(); ++i) {
                    matrix[i][col] = 0;
                }
                for (int j = 0; j < matrix[0].size(); ++j) {
                    matrix[row][j] = 0;
                }
            }
        }
    };

int main(){
    Solution sol;
    vector<vector<int>> matrix = {
        {1, 1, 1},
        {1, 0, 1},
        {1, 1, 1}
    };
    sol.setZeroes(matrix);
    for (const auto& row : matrix) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
    return 0;
}