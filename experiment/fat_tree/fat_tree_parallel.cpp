//
// Created by cuongbv on 29/02/2020.
//

#include <map>
#include <thread>
#include <iostream>
#include <mutex>
#include "../../node/FatTreeNode.h"
#include "../../graph/fattree/FatTree.h"

std::map<int, FatTreeNode*> fatTreeNodeList;
std::mutex mutex;

void createFatTreeNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        mutex.lock();
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
        mutex.unlock();
    }
}

void buildTable(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        fatTreeNodeList[i]->buildTable(numCoreSwitch, numPods, numSwitchPerPod);
    }
}

int main() {
    int paramK = 250;

    auto begin = std::chrono::system_clock::now();
    auto *fatTree = new FatTree(paramK);
    auto doneTopo = std::chrono::system_clock::now();

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

    auto doneAlgo = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

    return 0;
}