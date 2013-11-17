#include <iostream>

int main() {
    int a[100];
    for (int i = 0; i < 100; i++)
        a[i] = i;
    std::cout << "a['a'] = " << a['a'] << std::endl;
}
