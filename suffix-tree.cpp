#include "suffix-tree.h"
#include <cassert>

SuffixTreeNode::SuffixTreeNode() {
    for (int i = 0; i < CHAR_CODE_LIMIT; i++) {
        edges[i].from = this;
        edges[i].to   = NULL;
        edges[i].start_position  = -1;
        edges[i].end_position = 0;
        edges[i].first_char = i;
    }
}

SuffixTree::SuffixTree(std::string initString) {
    joker = new SuffixTreeNode;
    root  = new SuffixTreeNode;
    joker->suffix_link = NULL;
    joker->edges[0].to = joker;
    joker->parent_edge = joker->edges[0];
    for (int i = 1; i < CHAR_CODE_LIMIT; i++) {
        joker->edges[i].to   = root;
    }
    root->parent_edge = joker->edges[1];
    root->suffix_link = joker;
    current_node.edge = joker->edges[1];
    current_node.offset = 0;
    AddString(initString);
}

void SuffixTree::InNode(const char ch, bool while_end) {
    current_node.edge.to->edges[ch].to = new SuffixTreeNode;
    current_node.edge.to->edges[ch].first_char = ch;
    current_node.edge.to->edges[ch].start_position = static_cast<int>(str.size());
    current_node.edge.to->edges[ch].end_position = INFINITY;
    if (current_node.edge.from == joker)
    while_end = true;
    else
    current_node = GetSuffix(current_node);
}

void SuffixTree::AddLetter(const char letter) {
    bool while_end = false;
    while (!while_end) {
        if (current_node.edge.end_position == current_node.offset) {
            if (current_node.edge.to->edges[letter].to == NULL) {
                InNode(letter, while_end);
            } else {
                current_node.edge = current_node.edge.to->edges[letter];
                current_node.offset = current_node.edge.start_position + 1;
                if (current_node.edge.from == joker)
                while_end = true;
            }
        } else {
            if (str[current_node.offset] == letter) {
                ++current_node.offset;
                break;
            } else {
                SuffixTreeNode *new_node = SplitEdge(current_node);
                current_node.edge = new_node->parent_edge;
                InNode(letter, while_end);
            }
        }
    }
    str.push_back(letter);
}

SuffixTreePosition SuffixTree::GetSuffix(SuffixTreePosition &position) {
    SuffixTreeNode *cur = position.edge.from->suffix_link;
    if (cur == NULL) {
        return position;
    }
    int cur_offset = position.edge.start_position;
    while (true) {
        SuffixTreePosition res;
        assert(position.edge.end_position - cur_offset != 0);
        SuffixTreeEdge cur_edge = cur->edges[str[cur_offset]];
        cur_offset += cur_edge.end_position - cur_edge.start_position;
        if (cur_offset >= position.offset) {
            res.edge = cur_edge;
            res.offset = position.offset - cur_offset + cur_edge.end_position;
            res.offset = res.offset - cur_edge.start_position + cur_edge.start_position;
            return res;
        }
        cur = cur_edge.to;
    }
}

SuffixTreeNode* SuffixTree::SplitEdge(SuffixTreePosition position) {
    if (position.edge.end_position == position.offset)
    return position.edge.to;
    SuffixTreeNode *new_node = new SuffixTreeNode;
    SuffixTreeEdge new_edge;
    new_edge.start_position  = position.offset;
    new_edge.end_position    = position.edge.end_position;
    new_edge.first_char = str[position.offset];
    new_edge.from       = new_node;
    new_edge.to         = position.edge.to;
    new_node->edges[str[position.offset]] = new_edge;
    position.edge.end_position = position.offset;
    position.edge.to      = new_node;
    new_node->parent_edge = position.edge.from->edges[position.edge.first_char] = position.edge;
    new_node->suffix_link = SplitEdge(GetSuffix(position));
    return new_node;
}

SuffixTree::~SuffixTree() {
    delete joker;
    DestroyNode(root);
}

void SuffixTree::DestroyNode(SuffixTreeNode *node) {
    if (node == NULL) return;
    for (int i = 0; i < CHAR_CODE_LIMIT; i++) {
        DestroyNode(node->edges[i].to);
    }
    delete node;
}

void SuffixTree::AddString(const std::string str) {
    for (int i = 0; i < str.length(); i++) {
        AddLetter(str[i]);
    }
}

/*
std::vector<std::pair<int, int> > SuffixTree::GreatestCommonSuffix(std::string str) {
    std::vector<std::pair<int, int> > resultVector;
    SuffixTreePosition currentPosition;
    currentPosition.edge = joker->edges[1];
    currentPosition.offset = 0;
    int depth = 0;
    for (int i = 0; i < str.length(); i++) {
        while (true) {
            if (currentPosition.edge.end_position == currentPosition.offset) {
                if (currentPosition.edge.to->edges[str[i]].to != NULL) {
                    currentPosition.edge = currentPosition.edge.to->edges[str[i]];
                    currentPosition.offset = currentPosition.edge.start_position + 1;
                    break;
                }
                if (currentPosition.edge.from == joker) {
                    --depth;
                    break;
                }
                currentPosition = GetSuffix(currentPosition);
                --depth;
            } else {
                bool if_condition = (currentPosition.offset >= static_cast<int>(this->str.size()));
                if_condition = if_condition || (this->str[currentPosition.offset] != str[i]);
                if (if_condition) {
                    --depth;
                    currentPosition = GetSuffix(currentPosition);
                } else {
                    ++currentPosition.offset;
                    break;
                }
            }
        }
        depth++;
        resultVector.push_back(std::make_pair(depth, currentPosition.offset));
    }
    return resultVector;
}
*/

/*
//template<class Visitor>
std::vector<int> DFS(std::string pattern, int currentIndex){
    for (int i = 0; i < CHAR_CODE_LIMIT; i++) {
        if (
    }
}

std::vector<int> findAllOccurences(const SuffixTree& suffixTree, std::string pattern) {
    std::vector<int> result = DFS(pattern, 0);
    return result;
}*/
