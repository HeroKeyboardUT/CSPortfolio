#include "Hieudz.h"
// Symbol       Value
// I             1
// V             5
// X             10
// L             50
// C             100
// D             500
// M             1000
class Solution {
    public:
        string intToRoman(int num) {
            string result = "";
            string symbols[] = {"I", "IV", "V", "IX", "X", "XL", "L", "XC", "C", "CD", "D", "CM", "M"};
            int values[] = {1, 4, 5, 9, 10, 40, 50, 90, 100, 400, 500, 900, 1000};
            for (int i = 12; i >= 0; --i) {
                while (num >= values[i]) {
                    result += symbols[i];
                    num -= values[i];
                }
            }
            return result;
        }
    };
int main(){
    Solution s;
    cout << s.intToRoman(3) << endl; // Output: "III"
    cout << s.intToRoman(4) << endl; // Output: "IV"
    cout << s.intToRoman(9) << endl; // Output: "IX"
    cout << s.intToRoman(58) << endl; // Output: "LVIII"
    cout << s.intToRoman(1994) << endl; // Output: "MCMXCIV"
    return 0;
}