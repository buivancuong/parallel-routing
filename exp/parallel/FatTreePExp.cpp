//
// Created by cuongbv on 22/02/2020.
//

#include <thread>
#include "FatTreePExp.h"
#include "../../graph/fattree/FatTree.h"

void FatTreePExp::createFatTreeNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        this->fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }
}

void FatTreePExp::buildTable(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->fatTreeNodeList[i]->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
}

FatTreePExp::FatTreePExp(int paramK, int numSubThread) {
    auto *fatTree = new FatTree(paramK);
    int numCoreSwitch = fatTree->getNumCoreSwitch();
    int numPods = fatTree->getNumPods();
    int numSwitchPerPod = fatTree->getNumSwitchPerPod();

    int subSet = (int)(fatTree->getNumVertices() / numSubThread) + 1;
    int partition[numSubThread + 1];
    int start = 0;
    partition[0] = 0;
    for (int part = 1; part < (int)(sizeof(partition)/ sizeof(*partition)); ++part) {
        partition[part] = start + subSet;
        start = partition[part];
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&FatTreePExp::createFatTreeNodeList, this, partition[i], partition[i + 1], numCoreSwitch, numPods, numSwitchPerPod);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&FatTreePExp::buildTable, this, partition[i], partition[i + 1], numCoreSwitch, numPods, numSwitchPerPod);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }
}
