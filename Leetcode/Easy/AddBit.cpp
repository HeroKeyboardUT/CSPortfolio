#include "Hieudz.h"

class Solution {
    public:
        int addBit(int a, int b) {
            while (b != 0) {
                int carry = a & b; // Calculate carry
                a = a ^ b; // Sum without carry
                b = carry << 1; // Shift carry to the left
            }
            return a;
        }

};

int main(){
    Solution sol;
    int a = 5; // 0101 in binary
    int b = 3; // 0011 in binary
    int result = sol.addBit(a, b);
    cout << "Sum of " << a << " and " << b << " is: " << result << endl; // Output should be 8 (1000 in binary)
    return 0;
}