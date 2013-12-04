#pragma one

#include <vector>
#include <string>
#include <set>

class LexiconTrieNode;

class LexiconTrie {
    private:

    public:
        LexiconTrie();
        ~LexiconTrie();

        void buidTrie(const vector<string> &stringVector);
        bool isInTrie(const string &word);


}
;
