#include <iostream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <cmath>
#include <thread>
#include <pthread.h>
#include "graph/smallworld/SmallWorld2DGrid.h"
#include "node/Node.h"
#include "utils/CORRAUtils.h"
#include "node/CORRANode.h"
#include "graph/fattree/FatTree.h"
#include "node/FatTreeNode.h"
#include "exp/serial/CORRAv1SExp.h"
#include "exp/parallel/CORRAv1PExp.h"


int main() {
    int xTopoSize = 32;
    int yTopoSize = 32;
    int deltaNeighbor = 3;
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

    // std::cout << xBlockSize << " " << yBlockSize << std::endl;

    // Create Topology
    auto *corrAv1PExp = new CORRAv1SExp(xBlockSize, yBlockSize, xTopoSize, yTopoSize, deltaNeighbor, alphas);
    delete corrAv1PExp;

    return 0;
}