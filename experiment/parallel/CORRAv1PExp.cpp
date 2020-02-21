//
// Created by cuongbv on 21/02/2020.
//

#include <thread>
#include <iostream>
#include "CORRAv1PExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"

CORRAv1PExp::CORRAv1PExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float> &alphas, int numSubThread) {
//    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    int subSet = (int)(xTopoSize * yTopoSize / numSubThread) + 1;
    int partition[numSubThread + 1];
    int start = 0;
    partition[0] = 0;
    for (int part = 1; part < (int)(sizeof(partition)/ sizeof(*partition)); ++part) {
        partition[part] = start + subSet;
        start = partition[part];
    }

    std::vector<std::thread> threads;

    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::createNodeList, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto & thread : threads) {
        thread.join();
    }
}

void CORRAv1PExp::createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        auto *node = new CORRANode(i);
        nodeList.insert(std::pair<int, CORRANode*>(i, node));
    }
}

void
CORRAv1PExp::createNearFarNeighbors(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        nodeList[i]->prepareLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void CORRAv1PExp::createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        nodeList[i]->createLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void CORRAv1PExp::createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        nodeList[i]->createLocalRouting(xTopoSize);
    }
}
