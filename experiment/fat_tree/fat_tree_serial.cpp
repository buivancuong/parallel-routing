//
// Created by cuongbv on 29/02/2020.
//

#include <iostream>
#include <chrono>
#include "../../graph/fattree/FatTree.h"
#include "../../node/FatTreeNode.h"

int main() {
    int paramK = 250;

    auto begin = std::chrono::system_clock::now();
    auto *fatTree = new FatTree(paramK);
    auto doneTopo = std::chrono::system_clock::now();

    // std::cout << "create fat tree topo " << fatTree->getNumVertices() << " node" << std::endl;
    int numCoreSwitch = fatTree->getNumCoreSwitch();
    int numPods = fatTree->getNumPods();
    int numSwitchPerPod = fatTree->getNumSwitchPerPod();
    std::map<int, FatTreeNode*> fatTreeNodeList;

    int totalSwitch = numCoreSwitch + numPods * numSwitchPerPod;
    for (int i = 0; i < totalSwitch; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }

    for (std::pair<int, FatTreeNode*> fatTreeNode : fatTreeNodeList) {
        fatTreeNode.second->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
    // std::cout << "Done" << std::endl;

    auto doneAlgo = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

    return 0;
}