//
// Created by cuongbv on 29/02/2020.
//

#include <iostream>
#include <chrono>
#include <fstream>
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

    auto doneAlgo = std::chrono::system_clock::now();

    std::fstream routingTableFile;
    std::string localTableFileName ("./../experiment/fat_tree/k_" + std::to_string(paramK));
    routingTableFile.open(localTableFileName.c_str(), std::ios::out);
    for (std::pair<int, FatTreeNode*> fatTreeNode : fatTreeNodeList) {
        std::vector<std::pair<std::vector<int>, int> > routingTable = fatTreeNode.second->getRoutingTable();
        for (std::pair<std::vector<int>, int> destNodeID : routingTable) {
            std::string row (std::to_string(fatTreeNode.first) + " "
                + std::to_string(destNodeID.first[0]) + "."
                + std::to_string(destNodeID.first[1]) + "."
                + std::to_string(destNodeID.first[2]) + "."
                + std::to_string(destNodeID.first[3])
                + " " + std::to_string(destNodeID.second) + "\n");
            routingTableFile << row;
        }
    }
    routingTableFile.close();

    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

    return 0;
}