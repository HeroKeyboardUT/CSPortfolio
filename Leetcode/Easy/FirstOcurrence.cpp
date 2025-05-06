#include "Hieudz.h"

class Solution {
    public:
        int strStr(string haystack, string needle) {
            for(int i=0 ; i<haystack.size() ; i++){
                if(haystack[i] == needle[0]){
                    int j = 0;
                    while(j < needle.size() && i+j < haystack.size() && haystack[i+j] == needle[j]){
                        j++;
                    }
                    if(j == needle.size()){
                        return i;
                    }
                }
            }
            return -1;
        }
    };

int main(){
    Solution sol;
    string haystack = "hello";
    string needle = "ll";
    cout << sol.strStr(haystack, needle) << endl; // 2
    return 0;
}
