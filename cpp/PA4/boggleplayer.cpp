#include "boggleplayer.h"
//#include "boggleutil.h"
#include <iostream>

/* name:    buildLexicon
 * Load the words into an efficient data structure that will be used internally as needed by the BogglePlayer.
 * input:   set<string>, containing lexicon given, only lowercase letters a-z
 */
BogglePlayer::BogglePlayer() {
}
BogglePlayer::~BogglePlayer() {
}
void BogglePlayer::buildLexicon(const set<string>& word_list) { 
             }
/* name:    setBoard
 * input:   uint rows, uint cols
 */
void BogglePlayer::setBoard(unsigned int rows, unsigned int cols, string** diceArray) {
             }
/* name:    getAllValidWords
 * input:   at least min_word_length, set<string>* words
 * return:  false: setBoard() || buildLexicon() not called by boggleplayer
 *          ture : both been called
 */
bool BogglePlayer::getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
             }
/* name:    isInLexicon
 * input:   const string&
 * return:  ture if string found in the lexicon by buildLexicon()
 *          false if not found || buildLexicon() not called
 */
bool BogglePlayer::isInLexicon(const string& word_to_check) {
             }
// CHECKPOINT
/* name:    isOnBoard
 * input:   const string&
 * return:  vector<int>, index of array, mark order of string on board R*width+C
 *          empty vector<int> if not possible to form word || setboard() not called
 */
vector<int> BogglePlayer::isOnBoard(const string& word_to_check) {
             }
/* name:    getCustomBoard
 * for testing
 */
void BogglePlayer::getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols) {
             }
