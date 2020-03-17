#include <iostream>
#include <fstream>

int main() {

    int x = 1;
    int y = 2;

    std::fstream fstream;
    std::string fileName ("local_" + std::to_string(x) + "x" + std::to_string(y));


    std::cout << fileName << std::endl;

    return 0;
}