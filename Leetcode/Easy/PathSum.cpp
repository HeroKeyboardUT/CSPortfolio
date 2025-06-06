#include "Hieudz.h"

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

class Solution {
    public:
        bool hasPathSum(TreeNode* root, int targetSum) {
            int sum = 0;
            return hasPathSumHelper(root, targetSum, sum);
        }
        bool hasPathSumHelper(TreeNode* node, int targetSum, int sum) {
            if (node == nullptr) return false;
            cout << node->val << " " << sum << endl;
            sum += node->val;
            if (node->left == nullptr && node->right == nullptr) {
                return sum == targetSum;
            }
            return hasPathSumHelper(node->left, targetSum, sum) || hasPathSumHelper(node->right, targetSum, sum);
        }
    };

int main(){
    Solution sol;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    cout << sol.hasPathSum(root, 3) << endl; // 1
    return 0;
}