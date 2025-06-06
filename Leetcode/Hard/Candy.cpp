// There are n children standing in a line. Each child is assigned a rating value given in the integer array ratings.

// You are giving candies to these children subjected to the following requirements:

// Each child must have at least one candy.
// Children with a higher rating get more candies than their neighbors.
// Return the minimum number of candies you need to have to distribute the candies to the children.

#include "Hieudz.h"

class Solution {
public:
    int candy(vector<int>& ratings) {
        vector<int> dp(ratings.size(), 1);
        int sum = 0;
        for(int i = 1; i < ratings.size(); ++i) {
            if (ratings[i] > ratings[i - 1]) {
                dp[i] = dp[i - 1] + 1;
            }
        }
        for(int i = ratings.size() - 2; i >= 0; --i) {
            if (ratings[i] > ratings[i + 1]) {
                dp[i] = max(dp[i], dp[i + 1] + 1);
            }
        }
        for(int i = 0; i < dp.size(); ++i) {
            sum += dp[i];
        }

        return sum;
        
    }
};

int main() {
    Solution s;
    vector<int> ratings = {1, 0, 2};
    cout << s.candy(ratings) << endl; // Output: 5
    return 0;
}