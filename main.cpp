#include "suffix-tree.h"
#include <cstdio>
#include <string>
#include <iostream>
std::string str;

int main()
{
    std::cin >> str;
    SuffixTree tree(str);
    return 0;
}
