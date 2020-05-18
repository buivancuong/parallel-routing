//
// Created by cuongbv on 29/02/2020.
//

#include <cmath>
#include <thread>
#include <mutex>
#include <iostream>
#include <fstream>
#include "../../node/Node.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/CORRAUtils.h"

std::map<int, Node*> corra2NodeList;
std::mutex mutex;

void createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        Node *node = new Node(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
        }
        mutex.lock();
        corra2NodeList.insert(std::pair<int, Node *>(i, node));
        mutex.unlock();
    }
}

void addNearFarNeighbors(int startNodeID, int endNodeID, Graph *graph) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto source = graph->getAdjList()[i];
        for (std::pair<int, float> neighbor : source) {
            if (neighbor.second == 1) {
                mutex.lock();
                corra2NodeList[i]->addNearNeighbors(corra2NodeList[neighbor.first]);
                mutex.unlock();
            } else {
                mutex.lock();
                corra2NodeList[i]->addFarNeighbors(corra2NodeList[neighbor.first]);
                mutex.unlock();
            }
        }
    }
}

void prepareLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize);
    }
}

void createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize);
    }
}

void createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->createLocalRouting(xTopoSize);
    }
}

void findBR1(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->findBR1();
    }
}

void findBRn(int startNodeID, int endNodeID, int n) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->findToBRn(n);
    }
}

void broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void handleMissingBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        corra2NodeList[i]->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

int main() {
    int xTopoSize = 128;
    int yTopoSize = 128;
    int deltaNeighbor = 8;
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

    auto begin = std::chrono::system_clock::now();
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    auto doneTopo = std::chrono::system_clock::now();

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
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createNodeList, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(addNearFarNeighbors, partition[i], partition[i + 1], smallWorld2DGrid);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(prepareLocality, partition[i], partition[i + 1], deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
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
        std::thread thread(broadcastLocalBridge, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(handleMissingBridge, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
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

    auto doneAlgo = std::chrono::system_clock::now();

    std::fstream localTableFile;
    std::fstream blockTableFile;
    std::string localTableFileName ("./../experiment/corra_v2/local_" + std::to_string(xTopoSize) + "x" + std::to_string(yTopoSize) + "r" + std::to_string(deltaNeighbor));
    std::string blockTableFileName ("./../experiment/corra_v2/block_" + std::to_string(xTopoSize) + "x" + std::to_string(yTopoSize) + "r" + std::to_string(deltaNeighbor));

    localTableFile.open(localTableFileName.c_str(), std::ios::out);
    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        std::map<int, std::pair<int, double> > localRT = corraNode.second->getLocalRT();
        for (std::pair<int, std::pair<int, double> > destNodeID : localRT) {
            std::string row (std::to_string(corraNode.first) + " " + std::to_string(destNodeID.first) + " " + std::to_string(destNodeID.second.first) + "\n");
            localTableFile << row;
        }
    }
    localTableFile.close();

    blockTableFile.open(blockTableFileName.c_str(), std::ios::out);
    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        std::map<int, int> blockRT = corraNode.second->getBlockRT();
        for (std::pair<int, int> destBlock : blockRT) {
            std::string row (std::to_string(corraNode.first) + " " + std::to_string(destBlock.first) + " " + std::to_string(destBlock.second) + "\n");
            blockTableFile << row;
        }
    }
    blockTableFile.close();

    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

    return 0;
}