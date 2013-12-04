#include "boggleplayer.h"
//#include "boggleutil.h"
#include <iostream>

using namespace std;

// Constructors
//======================================================================
BogglePlayer::BogglePlayer() {
}
BogglePlayer::~BogglePlayer() {
    // clearBoard();
}

int BogglePlayer::getIndex(int row, int col) {
    // if (row < 0 || col < 0 || row >= ROWS || col >= COLS) {
    //     return false;
    // }
    return row * COLS + col;
}
    
std::string BogglePlayer::toLowercase(std::string str) {
    std::string res;
    for (std::string::iterator p = str.begin(); p != str.end(); p++)
            res += tolower(*p);
    return res;
}

/* name:    buildLexicon
 * Load the words into an efficient data structure that will be used internally as needed by the BogglePlayer.
 * input:   set<string>, containing lexicon given, only lowercase letters a-z
 */
void BogglePlayer::buildLexicon(const set<std::string>& word_list) { 
             }
/* name:    setBoard
 * input:   uint rows, uint cols
 */
void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, std::string** diceArray) {

    // BogglePlayer is the object
    if (isBoardSet) { clearBoard(); }
    ROWS = rows;
    COLS = cols;

    // you can allocate memory first in the heap, and assign values in diceArray in it.
    // assume your have a std::string** board and bool** board_used
    // this -> board = new std::string*[ROWS];
    // this -> isUsed = new bool*[ROWS];
    // for (int i = 0; i < ROWS; i++) {
    // this -> board[i] = new std::string[COLS];
    // this -> isUsed[i] = new bool[COLS];
    //
    // Note here, you may not assign new with board [ROWS][COLS]. use malloc instead
    //
    // then for (i, j) in (rows, cols), copy values from diceArray onto the board
    // this -> board[i][j] = toLowerCase(diceArray[i][j]);
    // this -> isUsed[i][j] = false;
    
    // instead use a linear vector to represent the matrix, 
    // and use getIndex to switch back
    // another thing good to use vector is vector.clear(), 
    // it's easier to destroy board

    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            board.push_back(toLowercase(diceArray[i][j]));
            isUsed.push_back(false);
        }
    }
    isBoardSet = true;
}

void BogglePlayer::clearBoard() {
    if (!isBoardSet) return;
    // if you're using new to create the board on the heap
    // then you need to delet[]
    // for (int i = 0; i < Rows; i++) {
    // delete[] board[i];
    // delete[] isUsed[i];
    // }
    // delete[] board; 
    // // delete this -> board = new std::string*[ROWS]; where you store your rows, very easy to forget
    // delete[] isUsed;

    board.clear();  // vector.clear()
    isUsed.clear(); // vector.clear()
    // ROWS = COLS = 0;
    isBoardSet = false;
}
/* name:    getAllValidWords
 * input:   at least min_word_length, set<string>* words
 * return:  false: setBoard() || buildLexicon() not called by boggleplayer
 *          ture : both been called
 */
bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<std::string>* words) {
             }
/* name:    isInLexicon
 * input:   const string&
 * return:  ture if string found in the lexicon by buildLexicon()
 *          false if not found || buildLexicon() not called
 */
bool BogglePlayer::isInLexicon(const std::string& word_to_check) {
             }
// CHECKPOINT
/* name:    isOnBoard
 * input:   const string&
 * return:  vector<int>, index of array, mark order of string on board R*width+C
 *          empty vector<int> if not possible to form word || setboard() not called
 */
std::vector<int> BogglePlayer::isOnBoard(const std::string& word_to_check) {
    std::vector<int> track;
    std::string word = toLowercase(word_to_check);
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (this -> searchBoard(word, i, j, track) ) { 
                // i, j is the starting point searchBoard()
                return track;
            }
        }
    }
    return track;
}
bool BogglePlayer::searchBoard(const std::string &word, int row, int col, vector<int> &track) {
    // Put this test here is better than in getIndex();
    // because there you would need to check if index >= board.size()
    if (row < 0 || col < 0 || row >= ROWS || col >= COLS) { 
        return false; 
    }
    int index = getIndex(row, col);
    if (isUsed[index]) { return false; }

    std::string curr = board[index]; // curr is the string on the die
    int currLen = (int)curr.length();
    int wordLen = (int)word.length();
    // string.length() gives by default unsigned int, cast to int to stop gcc bitching up

    // Step1: Before we set isUsed[index] to true
    // check if the letters is correct
    // case1: letters on this die is longer than word
    if (currLen > wordLen) { return false; } 
    // case2: letters on this die is not the initial segment of word
    if (word.substr(0, currLen) != curr) { return false; }
    // Kube uses substr().compare(curr) != 0

    // Step2: Now need to check the die
    // push back it's location to the vector and record it first
    track.push_back(index); 
    // Now we check if we've already found the whole word
    // case1: letters on this die match the whole word
    if (currLen == wordLen) { return true; }

    // case2: we didn't match the whole word yet, only it's first segment.
    // Set isUsed[index] to true, I'm going to use the die
    isUsed[index] = true;   

    // Need to check the rest of the word, use recurrsion
    std::string rest = word.substr(currLen);

    // iterate over its neighbors
    for(int nr = row - 1; nr <= row + 1; nr++) {
        for(int nc = col - 1; nc <= col + 1; nc++) {
            // bool res = searchBoard(rest, nr, nc, track)
            // if (res) {
            if (this -> searchBoard(rest, nr, nc, track)) {
                // We found our neighbour has the rest of the word 
                // and now vector track is filled
                // we need to put down the dice
                isUsed[index] = false;
                return true;
            }
        }
    }

    // case3: the letters on the die doesn't match the word
    // put down the die
    isUsed[index] = false;  
    // removes the last element in the vector, cuz we record this die's location at first
    track.pop_back();
    return false;
}
/* name:    getCustomBoard
 * for testing
 */
void BogglePlayer::getCustomBoard(std::string** &new_board, unsigned int *rows, unsigned int *cols) {
             }
