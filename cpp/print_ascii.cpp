#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
// test ifstream's good() bit
// input file name : "input"
// Needs to call twice to check if the next char is not eof()

int main(int argc, char* argv[]) {

           
    char* filename = argv[1];
    string control[] = { "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS", "HT", "LF", "VT", "FF", "CR", "SO", "SI", "DLE", "DC1", "DC2", "DC3", "DC4", "NAK", "SYN", "ETB", "CAN", "EM", "SUB", "ESC", "FS", "GS", "RS", "US", "DEL" };
    vector<string> name(256);
    vector<int> freqs(256);

    int total_count = 0;

    // initialize vector name
    for (int i = 0; i < name.size(); i++) {
        if (i < 32) {
            name[i] = control[i];
        }
        else if (32 < i  && i < 127) {
            name[i] = (unsigned char)i;
        }
        else if (i == 127) {
            name[i] = control[32];
        }
        else {
            name[i] = (unsigned char)i;
        }
        // use else if instead of if
        // else corresponds to the nearest if!
    }

    ifstream ifs(filename);
    unsigned char c;
    while (ifs.good()) {
        c = ifs.get();
        if (ifs.good()) { // if the next char is not eof()
        cout << c;
            total_count++;
            freqs[c]++;
        }
    }
    ifs.close();

    cout << ">>> EOF <<<" << endl;
    cout << endl;
    cout << "Total_count: " << total_count << endl;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i]) { 
            cout << "freqs['"<< i <<"',(" << name[i] << ")]: " << freqs[i] << endl;
        }
    }
}
