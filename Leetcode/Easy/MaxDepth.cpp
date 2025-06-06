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
        int maxDepth(TreeNode* root) {
            int depth = 0;
            return maxDepthHelper(root, depth);
        }
        int maxDepthHelper(TreeNode* node, int depth) {
            if (node==nullptr) return depth;
            depth++;
            int leftDepth = maxDepthHelper(node->left, depth);
            int rightDepth = maxDepthHelper(node->right, depth);
            return max(leftDepth, rightDepth);
        }
    };

int main(){
    Solution sol;
    TreeNode* root = new TreeNode(1);
    root->left = new TreeNode(2);
    root->right = new TreeNode(3);
    cout << sol.maxDepth(root) << endl; // 2
    return 0;
}