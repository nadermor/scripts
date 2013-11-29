#include "HCTree.hpp"
#include <cstring> // strcmp

char* prog;

void usage(char* prog_name) {
    cerr << prog_name << " called with incorrect arguments." << endl;
    cerr << "Usage: " << prog_name << " infile outfile"<< endl;
    exit(-1);

}

int main(int argc, char* argv[]) {

    if (argc != 3) usage(argv[0]);
    char* infile = argv[1];
    char* outfile = argv[2];


    // UT1: infile & outfile same name, should return error, OK
    // UT2: infile or outfile cannot open, should return error, OK
    //========================================================================
    if (!strcmp(infile,outfile)) {
        cerr << "Input file and Output file must be different!" << endl;
        exit(-1);
    }
    vector<int> freqs(256); // freqs = 256 zero's

    ifstream ifs(infile);
    // ifstream ifs (infile, ios::in | ios::binary);
    // if (!ifs) {
    // if (!ifs.is_open()) {
    if (!ifs.good()) {
        cerr << "File \"" << infile << "\" could not open." << endl;
        exit(-1);
    }

    ofstream ofs(outfile);
    // ofstream ofs (outfile, ios::out | ios::binary);
    if (!ofs.good()) {
        cerr << "File \"" << outfile << "\" could not be created." << endl;
        exit(-1);
    }
    //========================================================================

    int total_count = 0;
    unsigned char c;
    while (ifs.good()) {
        c = ifs.get();
        if (ifs.good()) { // if the next char is not eof()
            freqs[c]++;
            total_count++;
        }
    }

    cout << endl;
    cout << "Total_count: " << total_count << endl;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i]) { 
            cout << "freqs['"<< i <<"',(" << (unsigned char)i << ")]: " << freqs[i] << endl;
        }
    }

    // HCTree* tr = new HCTree();
    HCTree tr;
    tr.build(freqs);

    tr.encode(ifs.get(), ofs);

    ifs.close(); // need to read the file 2 times
    ofs.close();
}
