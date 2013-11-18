#include "HCTree.hpp"
#include <functional> // for less<HCNode>

HCTree::~HCTree() {
}

void HCTree::build(const vector<int>& freqs){

    // priority_queue<HCNode> pq; 
    // priority_queue<HCNode,vector<HCNode>,less<HCNode> > pq;
    // equivalent as above
    // need bool operator<(const HCNode& other) const; this func to be CONST!!!
    
    priority_queue<HCNode*,vector<HCNode*>,HCNodePtrComp> pq; 
    // This way is more correct, note this way is to compare pointers
    // priority_queue<HCNode*> pq; // have question on which less method it calls
    
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i]) { // only assign new nodes to non-zero freq letters
            this->leaves[i] = new HCNode(freqs[i], i); // initialize leaves[i] to a new node
            // this->count=freqs[i]; this->symbol=i;
            pq.push(this->leaves[i]);
            // pq is pushed with pointers!!
        }
    }
    // UT1: add 256 elems to pq, correct
    // UT2: print pq elems, priority order is correct

    // leaves_count = pq.size(); // might be zero
    while (pq.size() > 1)
    {
    HCNode* node0 = pq.top(); pq.pop();
    HCNode* node1 = pq.top(); pq.pop();
    cout << "pq pops out (" << node0->symbol << "," << node0->count << ")" << endl; 
    cout << "pq pops out (" << node1->symbol << "," << node1->count << ")" << endl; 

    //sum_count = node0->count + node1->count;
    HCNode* mergenode = new HCNode(node0->count + node1->count, 0x00, node0, node1);
    // this way is easier

    //HCNode* mergenode(sum_count, 0x00)
    node0->p = mergenode; //mergenode->c0 = node0;
    node1->p = mergenode; //mergenode->c1 = node1;

    pq.push(mergenode);
    cout << "pq pushes in (" << mergenode->symbol << "," << mergenode->count << ")" << endl << endl; 
    }

    //this->root = (!pq.empty()) ? pq.top() : 0;
    if (!pq.empty()) {
    this->root = pq.top(); // if pq.size() == 1, execute this line
    cout << "root is now: (" << root->symbol << "," << root->count << ")" << endl;
    }
    // else pq.size() == 0, root remain 0

}

    // UT3: test tree structure, correct
    // UT4: test empty tree, should have no leaves. root should remain 0, correct


//void HCTree::encode(byte symbol, BitOutputStream& out) const;

void trace (const HCNode* curr, ofstream& out) {
    if (!curr) return; // curr == nulptr; no code for it
    //if (!(curr->p)) return; // potential bug for one-node tree, results in no code at all
    trace (curr->p, out);
    if (curr == curr->p->c0) {
        out << '0';
    } else { 
        out << '1'; 
    }
}

void HCTree::encode(byte symbol, ofstream& out) const{
    // symbol will be cast to 0~255 index in the array
    
    trace(leaves[symbol], out);
}

//void HCTree::decode(BitInputStream& in) const;

int HCTree::decode(ifstream& in) const{
}
