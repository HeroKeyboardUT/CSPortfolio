#include "Hieudz.h"
class Solution {
    public:
        int mySqrt(int x) {
            for(long long i=0 ; i <= x ; i++){
                if(i*i == x){
                    return i;
                }
                else if(i*i > x){
                    return i-1;
                }
            }
            return 0;
        }
    };

int main(){
    Solution sol;
    cout << sol.mySqrt(8) << endl; // 2
    cout << sol.mySqrt(4) << endl; // 2
    cout << sol.mySqrt(16) << endl; // 4
}