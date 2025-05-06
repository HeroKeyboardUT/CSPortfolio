#include "Hieudz.h"
class Solution {
    public:
        string convert(string s, int numRows) {
            string result;
            if (numRows == 1 || numRows >= s.size()) return s;
            for(int i=0;i<numRows;i++){
                if (i == 0 || i == numRows - 1){
                    for(int j=i;j<s.size();j+=2*(numRows-1)){
                        result += s[j];
                    }
                } else {
                    for(int j=i;j<s.size();j+=2*(numRows-1)){
                        if (j < s.size()) result += s[j];
                        if (j + 2*(numRows-1) - 2*i < s.size()) result += s[j + 2*(numRows-1) - 2*i];
                    }
                }
            }
            return result;
        }
    };
int main(){
    Solution sol;
    cout << sol.convert("PAYPALISHIRING", 3) << endl; // "PAHNAPLSIIGYIR"
    cout << sol.convert("A", 1) << endl; // "A"
    cout << sol.convert("AB", 1) << endl; // "AB"
    cout << sol.convert("ABC", 1) << endl; // "ABC"
    cout << sol.convert("ABCD", 1) << endl; // "ABCD"
    return 0;
} 
// 1   7
// 2  68
// 3 5 9
// 4  10

// 1     9
// 2   8 10
// 3  7  11
// 4 6   12 
// 5     