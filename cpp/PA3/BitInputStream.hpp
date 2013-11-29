#include <iostream>

class BitInputStream {
    private:
        char buf;
        int nbits;
        std::istream& in;
            
    public:
            
            BitInputStream(std::istream& is) : in(is) {
                buf = 0;
                nbits = 8;
            }
            
            void fill() {
                buf = in.get();
                nbits = 0;
            }
            
            int readBit() {
            }
};
