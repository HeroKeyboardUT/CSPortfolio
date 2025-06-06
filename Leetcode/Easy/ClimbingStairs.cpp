// You are climbing a staircase. It takes n steps to reach the top.

// Each time you can either climb 1 or 2 steps. In how many distinct ways can you climb to the top?

#include "Hieudz.h"

class Solution {
    public:
        int climbStairs(int n) {
            int dp[46] = {0};
            dp[1] = 1;
            dp[2] = 2;
            for (int i = 3; i <= n; i++) {
                dp[i] = dp[i - 1] + dp[i - 2];
            }
            return dp[n];
        }
    };

int main() {
    Solution s;
    cout << s.climbStairs(5) << endl; // Output: 8
    return 0;
}

// 1 : 1
// 2 : 2
// 3 : 3
// 4 : 5
// 5 : 8
// 6 : 13   
// 7 : 21