#include "Hieudz.h"
struct TreeNode {
    public:
        int val;
        TreeNode *left;
        TreeNode *right;
        TreeNode() : val(0), left(nullptr), right(nullptr) {}
        TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
        TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};
class Solution {
    public:
        bool isSameTree(TreeNode* p, TreeNode* q) {
            return isSameTreeHelper(p, q);
        }
        bool isSameTreeHelper(TreeNode* p, TreeNode* q) {
            if (p == nullptr && q == nullptr) return true;
            if (p == nullptr || q == nullptr) return false;
            if (p->val != q->val) return false;
            return isSameTreeHelper(p->left, q->left) && isSameTreeHelper(p->right, q->right);
        }
    };
int main(){
    Solution sol;
    TreeNode* p = new TreeNode(1);
    p->left = new TreeNode(2);
    p->right = new TreeNode(3);
    TreeNode* q = new TreeNode(1);
    q->left = new TreeNode(2);
    q->right = new TreeNode(3);
    cout << sol.isSameTree(p, q) << endl; // 1
    return 0;
}