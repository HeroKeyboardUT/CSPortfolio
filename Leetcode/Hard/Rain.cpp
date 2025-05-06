// Given n non-negative integers representing an elevation map where the width of each bar is 1, 
// compute how much water it can trap after raining.

#include "Hieudz.h"
class Solution {
    public:
        int trap(vector<int>& height) {
            int maxHeight = 0;
            int count = 0;
            for(int i = 0; i < height.size(); ++i) {
                maxHeight = max(maxHeight, height[i]);
            }
            for(int i = 1; i <= maxHeight; ++i) {
                int left =  0;
                while(left < height.size() && height[left] < i) {
                    ++left;
                }
                for(int j = left + 1; j < height.size(); ++j) {
                    if(height[j] >= i) {
                        count += j - left - 1;
                        left = j;
                    }
                }
            }
            return count;
        }
        int trap1(vector<int>& height) {
            int n = height.size();
            if (n == 0) return 0;
            vector<int> leftMax(n), rightMax(n);
            int water = 0;
            leftMax[0] = height[0];
            for (int i = 1; i < n; ++i) {
                leftMax[i] = max(leftMax[i - 1], height[i]);
            }
            rightMax[n - 1] = height[n - 1];
            for (int i = n - 2; i >= 0; --i) {
                rightMax[i] = max(rightMax[i + 1], height[i]);
            }
            for (int i = 0; i < n; ++i) {
                water += min(leftMax[i], rightMax[i]) - height[i];
            }
            return water;
        }
        
    };

int main() {
    Solution s;
    vector<int> height = {4,2,0,3,2,5};
    // 0 0 0 0 0 1
    // 1 0 0 0 0 1
    // 1 0 0 1 0 1
    // 1 1 0 1 1 1
    // 1 1 0 1 1 1
    cout << s.trap(height) << endl; // Output: 9
    return 0;
}