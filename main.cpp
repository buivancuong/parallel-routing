#include <iostream>
#include <utility>
#include "./utils/VerticeUtils.h"
#include "./graph_entities/Edge.h"


int main() {

    int myInt[] = {1,2,3,5};
    std::vector<int> myvec (myInt, myInt + sizeof(myInt)/ sizeof(int));
    for (auto &item : myvec){
        std::cout << item << std::endl;
    }
    std::cout << myvec[0] << std::endl;

    std::cout << "Hello, World!" << std::endl;
    return 0;
}