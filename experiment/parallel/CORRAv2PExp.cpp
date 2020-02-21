//
// Created by cuongbv on 21/02/2020.
//

#include <thread>
#include "CORRAv2PExp.h"
#include "../../utils/CORRAUtils.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"

CORRAv2PExp::CORRAv2PExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float> &alphas, int numSubThread) {
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
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
        std::thread thread(&CORRAv2PExp::createNodeList, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::createNearFarNeighbors, this, partition[i], partition[i + 1], deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::createLocality, this, partition[i], partition[i + 1], deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::createLocalRouting, this, partition[i], partition[i + 1], xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::findBR1, this, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::findBRn, this, partition[i], partition[i + 1], 2);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::broadcastLocalBridge, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::handleMissingBridge, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv2PExp::updateBlockTable, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }
}

void CORRAv2PExp::createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        Node *node = new Node(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
        }
        this->corraNodeList.insert(std::pair<int, Node *>(i, node));
    }
}

void CORRAv2PExp::createNearFarNeighbors(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void CORRAv2PExp::createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void CORRAv2PExp::createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->createLocalRouting(xTopoSize);
    }
}

void CORRAv2PExp::findBR1(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->findBR1();
    }
}

void CORRAv2PExp::findBRn(int startNodeID, int endNodeID, int n) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->findToBRn(n);
    }
}

void CORRAv2PExp::broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    for (int i = startNodeID; i <= endNodeID; ++i) {
        this->corraNodeList[i]->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }
}

void CORRAv2PExp::handleMissingBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corraNodeList[i]->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void CORRAv2PExp::updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corraNodeList[i]->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

CORRAv2PExp::~CORRAv2PExp() = default;



