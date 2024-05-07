//
// Created by Toni on 1/21/2023.
//
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// KMP经典题目
class Solution {
  void genNext(string &mode, int next[]) {
    int j = 0;
    // j始终指向前缀末尾的下一位
    next[j] = 0;
    // i始终指向后缀末尾
    for (int i = 1; i < mode.size(); ++i) {
      while (j > 0 && mode[i] != mode[j]) {
        // 如果新的后缀与前缀不一样，
        j = next[j - 1];
      }
      if (mode[i] == mode[j])
        j++;
      // 此时j指向最长前缀的下一位，正好是最长前缀的长度
      next[i] = j;
    }
  }

public:
  int strStr(string haystack, string needle) {
    if (needle.size() == 0) {
      return 0;
    }
    int next[needle.size()];
    genNext(needle, next);

    int j = 0;
    for (int i = 0; i < haystack.size(); ++i) {
      while (j > 0 && haystack[i] != needle[j]) {
        j = next[j - 1];
      }
      if (haystack[i] == needle[j])
        j++;
      if (j == needle.size()) { // 文本串s里出现了模式串t
        return (i - needle.size() + 1);
      }
    }
    return -1;
  }
};

int main() { cout << Solution().strStr("aabaaabaaac", "aabaaac") << endl; }