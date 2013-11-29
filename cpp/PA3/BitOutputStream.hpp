#include <iostream>
class BitOutputStream {
    private:
        char buf;
        int nbits;
        std::ostream& out;
    public:
            BitOutputStream(std::ostream& os) : out(os), buf(0), nbits(0) {
            }
            
            void flush() {
                out.put(buf);
                out.flush();
                buf = nbits = 0;
            }
            
            void writeBit(int i) {
            }
};
