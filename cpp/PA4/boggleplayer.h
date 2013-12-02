#pragma once

#include "baseboggleplayer.h"

#include <set>
#include <string>
#include <vector>
// #include <map slow version

using std::set;
using std::string;
using std::vector;

class BogglePlayer : public BaseBogglePlayer {
    private:
    public:
        BogglePlayer();
        ~BogglePlayer();

void buildLexicon(const set<string> &word_list);
void setBoard(unsigned int rows, unsigned int cols, string** diceArray);
bool getAllValidWords(unsigned int minimum_word_length, set<string>* words) ;
bool isInLexicon(const string &word_to_check);
vector<int> isOnBoard(const string &word_to_check);
void getCustomBoard(string** &new_board, unsigned int* rows, unsigned int* cols);
};
