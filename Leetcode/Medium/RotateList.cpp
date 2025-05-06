#include "Hieudz.h"

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};


class Solution {
    public:
        ListNode* rotateRight(ListNode* head, int k) {
            ListNode* tail = head;
            if (!head || !head->next || k == 0) return head;
            int length = 1;
            while (tail->next) {
                tail = tail->next;
                length++;
            }
            k = k % length; 
            if (k == 0) {
                return head;
            }
            ListNode* newTail = head;
            for (int i = 1; i < length - k; i++) {
                newTail = newTail->next;
            }
            ListNode* newHead = newTail->next;
            newTail->next = nullptr;
            tail->next = head;
            return newHead;
        }
    };


int main(){
    Solution sol;
    ListNode* head = new ListNode(1, new ListNode(2, new ListNode(3, new ListNode(4, new ListNode(5)))));
    int k = 2;
    ListNode* rotatedHead = sol.rotateRight(head, k);
    while (rotatedHead) {
        cout << rotatedHead->val << " ";
        rotatedHead = rotatedHead->next;
    }
    cout << endl;
    return 0;
}