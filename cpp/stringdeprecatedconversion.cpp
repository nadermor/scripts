#include <iostream>

int main(int argc, char* argv[]) {
    char *Type = new char[10];
    std::cout << Type << std::endl;
    Type = "Access";
    std::cout << Type << std::endl;

    return 0;
}
