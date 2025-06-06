#include "Hieudz.h"

// A path in a binary tree is a sequence of nodes where each pair of adjacent nodes in the 
// sequence has an edge connecting them. A node can only appear in the sequence at most once.
// Note that the path does not need to pass through the root.
// The path sum of a path is the sum of the node's values in the path.
// Given the root of a binary tree, return the maximum path sum of any non-empty path.

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
        int maxPathSum(TreeNode* root) {
            int maxSum = INT_MIN;
            maxPathSumHelper(root, maxSum);
            return maxSum;

        }
        int maxPathSumHelper(TreeNode* node, int& maxSum) {
            if (node == nullptr) return 0;
            int left = max(0, maxPathSumHelper(node->left, maxSum));
            int right = max(0, maxPathSumHelper(node->right, maxSum));
            maxSum = max(maxSum, left + right + node->val);
            return node->val + max(left, right);
        }
       
    };


int main(){
    Solution sol;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    cout << sol.maxPathSum(root) << endl; // 6
    return 0;
}
//                    6
//                 /     \
//               2         -8
//              / \       / \
//             1   3     4   5
        