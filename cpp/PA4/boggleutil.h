#pragma once

#include <vector>
#include <string>
#include <set>

class LexiconTrieNode {
    private:
        char const data;
        LexiconTrieNode* parent;
        LexiconTrieNode* left;
        LexiconTrieNode* middle;
        LexiconTrieNode* right;
        bool isEnd;
    public:
        LexiconTrieNode(const char &data) : data(data),
        parent(0),
        left(0),
        middle(0),
        right(0),
        isEnd (false)
    {}

};


class LexiconTrie {
    private:
        LexiconTrieNode* root;
        unsigned int isize;

    public:
        LexiconTrie() : root(0), isize(0) {};
        ~LexiconTrie();

        // void buildLexicon(const std::vector<std::string> &stringVector);
        // bool isInLexicon(const std::string &word);


};
