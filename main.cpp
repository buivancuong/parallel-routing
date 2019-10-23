#include <iostream>
#include "./utils/VerticeUtils.h";

int main() {
//    auto* vu = new VerticeUtils();
    int x = VerticeUtils::getVerticeHorizontal(13,4);
    std::cout << x << "Hello, World!" << std::endl;
    return 0;
}