#include "Hieudz.h"
// You are given two non-empty linked lists representing two non-negative integers. 
// The digits are stored in reverse order, and each of their nodes contains a single digit. 
// Add the two numbers and return the sum as a linked list.
// You may assume the two numbers do not contain any leading zero, except the number 0 itself.
struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};
class Solution {
    public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
            long long num1 = 0 ; long long num2 = 0;
            int len1 = 0, len2 = 0;
            ListNode* p1 = l1;
            ListNode* p2 = l2;
            while(p1!=nullptr){
                p1 = p1->next;
                len1++;
            }
            while(p2!=nullptr){
                p2 = p2->next;
                len2++;
            }
            p1 = l1;
            p2 = l2;
            int c1=len1 ,c2=len2;
            while(p1!=nullptr){
                num1 += p1->val * pow(10, len1-c1);
                p1 = p1->next;
                c1--;
            }
            while(p2!=nullptr){
                num2 += p2->val * pow(10, len2-c2);
                p2 = p2->next;
                c2--;
            }
            int sum = num1 + num2;
            ListNode* result = new ListNode(0);
            ListNode* current = result;
            if(sum==0){
                return result;
            }
            while(sum > 0) {
                current->next = new ListNode(sum % 10);
                sum /= 10;
                current = current->next;
            }
            return result->next; 

        }
        ListNode* addTwoNumbers1(ListNode* l1, ListNode* l2) {
            ListNode* dummy = new ListNode(0);
            ListNode* current = dummy;
            int carry = 0;
            
            // Add digits while we have nodes in either list
            while (l1 != nullptr || l2 != nullptr || carry > 0) {
                int sum = carry;
                
                // Add value from l1 if available
                if (l1 != nullptr) {
                    sum += l1->val;
                    l1 = l1->next;
                }
                
                // Add value from l2 if available
                if (l2 != nullptr) {
                    sum += l2->val;
                    l2 = l2->next;
                }
                
                // Calculate digit and carry
                carry = sum / 10;
                int digit = sum % 10;
                
                // Add new node with current digit
                current->next = new ListNode(digit);
                current = current->next;
                }
                
                return dummy->next;
        }
    };

int main(){
    ListNode* l1 = new ListNode(2, new ListNode(4, new ListNode(3)));
    ListNode* l2 = new ListNode(5, new ListNode(6, new ListNode(4)));
    Solution s;
    ListNode* result = s.addTwoNumbers(l1, l2);
    while(result != nullptr) {
        cout << result->val << " ";
        result = result->next;
    }
    return 0;
}
// Output: 7 -> 0 -> 8 (342 + 465 = 807)