
#include "Hieudz.h"
class Solution {
    public:
        vector<int> twoSum(vector<int>& nums, int target) {
            for(int i = 0; i < nums.size(); i++) {
                for(int j = i + 1; j < nums.size(); j++) {
                    if(nums[i] + nums[j] == target) {
                        return {i, j};
                    }
                }
            }
            return {-1, -1}; 
        }
        vector<int> twoSum(vector<int>& nums, int target) {
            unordered_map<int, int> numMap; 
            for (int i = 0; i < nums.size(); i++) {
                int complement = target - nums[i]; 
                if (numMap.find(complement) != numMap.end()) {
                    return {numMap[complement], i}; 
                }
                numMap[nums[i]] = i; 
            }
            return {-1, -1}; 
        }
    };

int main(){
    int arr [] = {2, 7, 11, 15};
    vector<int> nums(arr, arr + sizeof(arr) / sizeof(int));
    int target = 9;
    Solution s;
    vector<int> result = s.twoSum(nums, target);
    cout << "Indices: " << result[0] << ", " << result[1] << endl;
    return 0;
}

