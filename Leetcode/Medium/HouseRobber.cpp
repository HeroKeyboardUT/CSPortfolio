#include "Hieudz.h"
// You are a professional robber planning to rob houses along a street.
// Each house has a certain amount of money stashed, 
// the only constraint stopping you from robbing each of them is that adjacent houses have security systems connected 
// and it will automatically contact the police if two adjacent houses were broken into on the same night.

// Given an integer array nums representing the amount of money of each house, 
// return the maximum amount of money you can rob tonight without alerting the police.

class Solution {
    public:
        int rob(vector<int>& nums) {
            if (nums.empty()) return 0;
            if (nums.size() == 1) return nums[0];
            if (nums.size() == 2) return max(nums[0], nums[1]);
            
            vector<int> dp(nums.size(), 0);
            dp[0] = nums[0];
            dp[1] = max(nums[0], nums[1]);
            
            for (int i = 2; i < nums.size(); i++) {
                dp[i] = max(dp[i - 1], dp[i - 2] + nums[i]);
            }
            return dp.back();
        };

};

int main() {
    Solution s;
    vector<int> nums = {2, 7, 9, 3, 1};
    cout << s.rob(nums) << endl; // Output: 12
    return 0;
}