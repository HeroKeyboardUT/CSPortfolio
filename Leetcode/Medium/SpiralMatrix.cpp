#include "Hieudz.h"


class Solution {
    public:
        vector<int> spiralOrder(vector<vector<int>>& matrix) {
            vector<int> result;
            if (matrix.empty() || matrix[0].empty()) return result;
            int top = 0, bottom = matrix.size() - 1, left = 0, right = matrix[0].size() - 1;
            while (top <= bottom && left <= right) {
                for (int i = left; i <= right; ++i) result.push_back(matrix[top][i]);
                ++top;
                for (int i = top; i <= bottom; ++i) result.push_back(matrix[i][right]);
                --right;
                if (top <= bottom) {
                    for (int i = right; i >= left; --i) result.push_back(matrix[bottom][i]);
                    --bottom;
                }
                if (left <= right) {
                    for (int i = bottom; i >= top; --i) result.push_back(matrix[i][left]);
                    ++left;
                }
            }
            return result;
        }
    };


int main(){
    Solution sol;
    vector<vector<int>> matrix = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    vector<int> result = sol.spiralOrder(matrix);
    for (int num : result) {
        cout << num << " ";
    }
    cout << endl;
    return 0;
}