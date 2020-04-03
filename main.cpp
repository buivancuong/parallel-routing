#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "graph/smallworld/SmallWorld2DGrid.h"
#include "utils/CORRAUtils.h"

int main() {

    int xTopoSize = 64;
    int yTopoSize = 64;
    int deltaNeighbor = 6;
    std::vector<float> alphas = {1.6, 2};

    int xBlockSize, yBlockSize;
    if ((int) (log2(xTopoSize)) % 2 == 0) {
        xBlockSize = (int) sqrt(xTopoSize);
    } else {
        xBlockSize = (int) sqrt(xTopoSize / 2);
    }
    if ((int) (log2(yTopoSize)) % 2 == 0) {
        yBlockSize = (int) sqrt(yTopoSize);
    } else {
        yBlockSize = (int) sqrt(yTopoSize / 2);
    }
    int numBlock = (xTopoSize / xBlockSize) * (yTopoSize / yBlockSize);

    int gridHop = CORRAUtils::getGridHop(0, 200, xTopoSize);
    std::cout << gridHop << std::endl;
//    int nodeBlock = CORRAUtils::getNodeBlock(10, xBlockSize, yBlockSize, xTopoSize);
//    std::cout << nodeBlock << std::endl;

    return 0;
}