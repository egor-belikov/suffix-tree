#pragma once
#include <vector>
#include <climits>
#include <string>
#include <utility>

const int CHAR_CODE_LIMIT = 256;
const int INFINITY = INT_MAX / 2;

struct SuffixTreeNode;

struct SuffixTreeEdge {
    SuffixTreeNode *from, *to;
    int start_position, end_position;
    char first_char;
};

struct SuffixTreeNode {
    SuffixTreeNode *suffix_link;
    SuffixTreeEdge edges[CHAR_CODE_LIMIT], parent_edge;
    SuffixTreeNode();
};

struct SuffixTreePosition {
    SuffixTreeEdge edge;
    int offset;
};

class SuffixTree {
    protected:
        SuffixTreeNode *joker, *root;
    private:
        SuffixTreePosition current_node;
        void InNode(const char ch, bool while_end);
        SuffixTreePosition GetSuffix(SuffixTreePosition &position);
        SuffixTreeNode* SplitEdge(SuffixTreePosition position);
        void DestroyNode(SuffixTreeNode *node);
    public:
        std::vector<char> str;
        explicit SuffixTree(std::string initString);
        void AddLetter(char ch);
        void AddString(const std::string str);
        //std::vector<std::pair<int, int> > GreatestCommonSuffix(std::string str);
        ~SuffixTree();
        //template<class Visitor>
        //void DFS(std::string pattern);
};

//std::vector<int> findAllOccurences(const SuffixTree& suffixTree, std::string pattern);
