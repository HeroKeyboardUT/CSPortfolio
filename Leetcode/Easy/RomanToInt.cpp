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
        int romanToInt(string s) {
            int result = 0;
            for(int i=0 ;i<s.size(); i++){
                if(s[i] == 'I'){
                    if(i+1 < s.size() && (s[i+1] == 'V' || s[i+1] == 'X')){
                        result -= 1;
                    }else{
                        result += 1;
                    }
                }else if(s[i] == 'V'){
                    result += 5;
                }else if(s[i] == 'X'){
                    if(i+1 < s.size() && (s[i+1] == 'L' || s[i+1] == 'C')){
                        result -= 10;
                    }else{
                        result += 10;
                    }
                }else if(s[i] == 'L'){
                    result += 50;
                }else if(s[i] == 'C'){
                    if(i+1 < s.size() && (s[i+1] == 'D' || s[i+1] == 'M')){
                        result -= 100;
                    }else{
                        result += 100;
                    }
                }else if(s[i] == 'D'){
                    result += 500;
                }else if(s[i] == 'M'){
                    result += 1000;
                }
            }
            return result;
        }
    };
int main(){
    Solution s;
    cout << s.romanToInt("III") << endl; // Output: 3
    cout << s.romanToInt("IV") << endl; // Output: 4
    cout << s.romanToInt("IX") << endl; // Output: 9
    cout << s.romanToInt("LVIII") << endl; // Output: 58
    cout << s.romanToInt("MCMXCIV") << endl; // Output: 1994
    return 0;
}