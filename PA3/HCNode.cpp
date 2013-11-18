#include "HCNode.hpp"

// bool comp(HCNode* one, HCNode* other) {
//     return *one < *other;
// }
// unnecessary method

bool HCNode::operator<(const HCNode& other) { // this has less priority than other
    if (this->count == other.count)
        return this->symbol > other.symbol;
    // put the greater letters to the back
    else
        return this->count > other.count;
}
