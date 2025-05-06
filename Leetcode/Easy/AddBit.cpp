#include "Hieudz.h"

class Solution {
    public:
        // string addBinary(string a, string b) {
        //     string result = "";
        //     int carry = 0;
        //     while(a!="" && b!=""){
        //         int s1 = a.back() - '0';
        //         int s2 = b.back() - '0';
        //         int sum = s1 + s2 + carry;
        //         if(sum >= 2){
        //             carry = 1;
        //         }else{
        //             carry = 0;
        //         }
        //         result += to_string(sum % 2);
        //         a.pop_back();
        //         b.pop_back();
        //         if(a == "" && b == ""){
        //             if(carry == 1){
        //                 result += "1";
        //             }
        //             reverse(result.begin(), result.end());
        //             return result;
        //         }           
        //     }
        //     if (a == ""){
        //         while(b != ""){
        //             int s2 = b.back() - '0';
        //             int sum = s2 + carry;
        //             if(sum >= 2){
        //                 carry = 1;
        //             }else{
        //                 carry = 0;
        //             }
        //             result += to_string(sum % 2);
        //             b.pop_back();
        //         }
        //         if(carry == 1){
        //             result += "1";
        //         } 
        //     }
        //     else if (b == ""){
        //         while(a != ""){
        //             int s1 = a.back() - '0';
        //             int sum = s1 + carry;
        //             if(sum >= 2){
        //                 carry = 1;
        //             }else{
        //                 carry = 0;
        //             }
        //             result += to_string(sum % 2);
        //             a.pop_back();
        //         }
        //         if(carry == 1){
        //             result += "1";
        //         } 
        //     }
        //     reverse(result.begin(), result.end());
        //     return result;
        // }
        string addBinary(string a, string b){
            string result = "";
            int carry = 0;
            int i = a.size() - 1, j = b.size() - 1;
            while(i >= 0 || j >= 0 || carry > 0){
                int sum = carry;
                if(i >= 0) sum += a[i--] - '0';
                if(j >= 0) sum += b[j--] - '0';
                result += to_string(sum % 2);
                carry = sum / 2;
            }
            reverse(result.begin(), result.end());
            return result;
        }
    };
int main() {
    Solution s;
    string a = "1010", b = "1011";
    cout << s.addBinary(a, b) << endl; // Output: "10101"
    return 0;
}