#include <iostream>

int main(int argc, char* argv[]) {
    char *Type = new char[10];
    std::cout << Type << std::endl;
    Type = "Access";
    std::cout << Type << std::endl;
    // stringdeprecatedconversion.cpp:6:10: 
    // warning: deprecated conversion from string constant to ‘char*’ [-Wwrite-strings]
    return 0;
}
