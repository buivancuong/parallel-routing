//
// Created by cuongbv on 29/02/2020.
//

#include <map>
#include <thread>
#include "../../node/FatTreeNode.h"
#include "../../graph/fattree/FatTree.h"

std::map<int, FatTreeNode*> fatTreeNodeList;

void createFatTreeNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }
}

void buildTable(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        fatTreeNodeList[i]->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
}

int main() {
    int paramK = 100;
    auto *fatTree = new FatTree(paramK);
    int numCoreSwitch = fatTree->getNumCoreSwitch();
    int numPods = fatTree->getNumPods();
    int numSwitchPerPod = fatTree->getNumSwitchPerPod();

    int numSubThread = 4;
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
        std::thread thread(createFatTreeNodeList, partition[i], partition[i + 1], numCoreSwitch, numPods, numSwitchPerPod);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(buildTable, partition[i], partition[i + 1], numCoreSwitch, numPods, numSwitchPerPod);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}