// Given two strings ransomNote and magazine, 
// return true if ransomNote can be constructed by using the letters from magazine and false otherwise.

// Each letter in magazine can only be used once in ransomNote.

#include "Hieudz.h"
class Solution {
    public:
        bool canConstruct(string ransomNote, string magazine) {
            unordered_map<char, int> charCount;
            for(char c : magazine) {
                charCount[c]++;
            }
            for(char c : ransomNote) {
                if(charCount[c] == 0) {
                    return false; 
                }
                charCount[c]--;
            }
            return true; 
        }
    };

int main() {
    Solution solution;
    string ransomNote = "a", magazine = "b";
    cout << solution.canConstruct(ransomNote, magazine) << endl; // Output: false
    ransomNote = "aa", magazine = "ab";
    cout << solution.canConstruct(ransomNote, magazine) << endl; // Output: false
    ransomNote = "aa", magazine = "aab";
    cout << solution.canConstruct(ransomNote, magazine) << endl; // Output: true
    return 0;
}
// The code defines a class Solution with a method canConstruct that checks if a ransom note can be constructed from a magazine string.