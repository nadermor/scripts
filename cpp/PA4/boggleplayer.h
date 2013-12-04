#pragma once

#include "baseboggleplayer.h"
#include "boggleutil.h"

#include <set>
#include <string>
#include <vector>
// #include <map slow version

using std::set;
using std::string;
using std::vector;

class BogglePlayer : public BaseBogglePlayer {
    private:
        // information of board
        bool isBoardSet;
        int ROWS, COLS;
        vector<string> board;
        vector<bool> isUsed;

        void clearBoard();
        
        // isOnBoard() returns a vector<int>
        bool searchBoard(const string &word, int row, int col, std::vector<int> &track);
           
        // helper functions
        int getIndex(int row, int col);
        string toLowercase (string st);
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
