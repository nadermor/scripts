#include "baseboggleplayer.h"

class BogglePlayer : public BaseBogglePlayer {
    public:
        BogglePlayer () {}
        ~BogglePlayer () {}

        /* name:    buildLexicon
         * Load the words into an efficient data structure that will be used internally as needed by the BogglePlayer.
         * input:   set<string>, containing lexicon given, only lowercase letters a-z
         */
        void buildLexicon(const set<string>& word_list) {
        }
        /* name:    setBoard
         * input:   uint rows, uint cols
         */
        void setBoard(unsigned int rows, unsigned int cols, string** diceArray) {
        }
        /* name:    getAllValidWords
         * input:   at least min_word_length, set<string>* words
         * return:  false: setBoard() || buildLexicon() not called by boggleplayer
         *          ture : both been called
         */
        bool getAllValidWords(unsigned int minimum_word_length, set<string>* words) {
        }
        /* name:    isInLexicon
         * input:   const string&
         * return:  ture if string found in the lexicon by buildLexicon()
         *          false if not found || buildLexicon() not called
         */
        bool isInLexicon(const string& word_to_check) {
        }
        
        // CHECKPOINT
        /* name:    isOnBoard
         * input:   const string&
         * return:  vector<int>, index of array, mark order of string on board R*width+C
         *          empty vector<int> if not possible to form word || setboard() not called
         */
        vector<int> isOnBoard(const string& word_to_check) {
        }
        /* name:    getCustomBoard
         * for testing
         */
        void getCustomBoard(string** &new_board, unsigned int *rows, unsigned int *cols) {
        }
};
