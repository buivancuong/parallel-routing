//
// Created by cuongbv on 28/02/2020.
//

#include <cmath>
#include <thread>
#include <mutex>
#include "../../node/CORRANode.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/CORRAUtils.h"

std::map<int, CORRANode*> corra1NodeList;
std::mutex mutex;

void createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *corraNode = new CORRANode(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            corraNode->setCentered(true);
        }
        mutex.lock();
        corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
        mutex.unlock();
    }
}

void addNearFarNeighbors(int startNodeID, int endNodeID, Graph *graph) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto source = graph->getAdjList()[i];
        for (std::pair<int, float> neighbor : source) {
            if (corra1NodeList[neighbor.first] == nullptr) continue;
            if (neighbor.second == 1) {
                mutex.lock();
                corra1NodeList[i]->addNearNeighbors(corra1NodeList[neighbor.first]);
                mutex.unlock();
            } else {
                mutex.lock();
                corra1NodeList[i]->addFarNeighbors(corra1NodeList[neighbor.first]);
                mutex.unlock();
            }
        }
    }
}

void prepareLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->prepareLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->createLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->createLocalRouting(xTopoSize);
    }
}

void findBR1(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->findBR1();
    }
}

void findBRn(int startNodeID, int endNodeID, int n) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->findToBRn(n);
    }
}

void broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }
}

void createGlobalTraceMap(int startNodeID, int endNodeID, Graph *topo) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->createGlobalTraceMap(topo);
    }
}

void updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra1NodeList[i]->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize, corra1NodeList);
    }
}

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
    int numSubThread = 4;

    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    int subSet = (int)(xTopoSize * yTopoSize / numSubThread);
    if (xTopoSize * yTopoSize - numSubThread * subSet != 0) subSet++;
    int partition[numSubThread + 1];
    int start = 0;
    partition[0] = 0;
    for (int part = 1; part < (int)(sizeof(partition)/ sizeof(*partition)); ++part) {
        partition[part] = start + subSet;
        start = partition[part];
    }

    std::vector<std::thread> threads;
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(createNodeList, partition[i], partition[i+1], xBlockSize, yBlockSize, xTopoSize, yTopoSize)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(addNearFarNeighbors, partition[i], partition[i+1], smallWorld2DGrid)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
//        threads.emplace_back(&CORRAv1PExp::prepareLocality, this, partition[i], partition[i + 1], deltaNeighbor, xTopoSize, yTopoSize);
        std::thread thread(prepareLocality, partition[i], partition[i + 1], deltaNeighbor, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createLocality, partition[i], partition[i + 1], deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createLocalRouting, partition[i], partition[i + 1], xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(findBR1, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(findBRn, partition[i], partition[i + 1], 2);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(broadcastLocalBridge, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createGlobalTraceMap, partition[i], partition[i + 1], smallWorld2DGrid);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(updateBlockTable, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    return 0;
}