#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

int main() {

    vector<int> freqs(256);

    int total_count = 0;
    ifstream ifs("input");
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

    cout << endl;
    cout << endl;
    cout << "Total_count: " << total_count << endl;
    for (int i = 0; i < freqs.size(); i++) {
        if (freqs[i]) { 
            cout << "freqs['"<< i <<"',(" << (unsigned char)i << ")]: " << freqs[i] << endl;
        }
    }
}
