#include "Hieudz.h"
// You are given a grid of cells, where each cell can be either live (1) or dead (0).
// Any live cell with fewer than two live neighbors dies as if caused by under-population.
// Any live cell with two or three live neighbors lives on to the next generation.
// Any live cell with more than three live neighbors dies, as if by over-population.
// Any dead cell with exactly three live neighbors becomes a live cell, as if by reproduction.

class Solution {
    public:
        void gameOfLife(vector<vector<int>>& board) {
            int rows = board.size();
            int cols = board[0].size();
            vector<vector<int>> temp(rows, vector<int>(cols, 0));
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    int liveNeighbors = 0;
                    for(int x = -1; x <= 1; x++){
                        for(int y = -1; y <= 1; y++){
                            if(x == 0 && y == 0) continue; 
                            if(i + x < 0 || i + x >= rows || j + y < 0 || j + y >= cols) continue; 
                            int newRow = i + x;
                            int newCol = j + y;
                            if(board[newRow][newCol] == 1){
                                liveNeighbors++;
                            }
                        }
                    }
                    int deadNeighbors = 8 - liveNeighbors;
                    if(board[i][j] == 1){
                        if(liveNeighbors < 2 || liveNeighbors > 3){
                            temp[i][j] = 0; 
                        } else {
                            temp[i][j] = 1; 
                        }
                    } else {
                        if(liveNeighbors == 3){
                            temp[i][j] = 1; 
                        } else {
                            temp[i][j] = 0; 
                        }
                    }
                }
            }
            for(int i = 0; i < rows; i++){
                for(int j = 0; j < cols; j++){
                    board[i][j] = temp[i][j]; 
                }
            }
            
        }
    };
int main() {
    Solution s;
    vector<vector<int>> board = {{0, 1, 0}, {0, 0, 1}, {1, 1, 1}, {0, 0, 0}};
    s.gameOfLife(board);
    for (const auto& row : board) {
        for (const auto& cell : row) {
            cout << cell << " ";
        }
        cout << endl;
    }
    return 0;
}