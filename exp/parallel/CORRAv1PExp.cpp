//
// Created by cuongbv on 21/02/2020.
//

#include <thread>
#include <iostream>
#include "CORRAv1PExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"


CORRAv1PExp::CORRAv1PExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float> &alphas, int numSubThread) {
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
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
//    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
//        threads.emplace_back(&CORRAv1PExp::createNodeList, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        std::thread thread(&CORRAv1PExp::createNodeList, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }
    std::cout << "Done create Node list" << std::endl;

    std::map<int, std::map<int, float> > tempAdj = topo->getAdjList();
    std::cout << "size " << tempAdj.size() << std::endl;
    for (std::pair<int, std::map<int, float> > sourceNode : tempAdj) {
        std::cout << " * ";
        std::map<int, float> tempNeighbor = sourceNode.second;
        for (std::pair<int, float> neighbor: tempNeighbor) {
            std::cout << " ** ";
            if (neighbor.second == 1) {
                std::cout << "node " << sourceNode.first << " neighbor " << neighbor.first << " near ";
                std::cout << corra1NodeList[neighbor.first]->getNodeID() << std::endl;
                corra1NodeList[sourceNode.first]->addNearNeighbors(corra1NodeList[neighbor.first]);
            } else {
                std::cout << "node " << sourceNode.first << " neighbor " << neighbor.first << " far ";
                std::cout << corra1NodeList[neighbor.first]->getNodeID() << std::endl;
                corra1NodeList[sourceNode.first]->addFarNeighbors(corra1NodeList[neighbor.first]);
            }
        }
    }
    std::cout << "Done add near far neighbors" << std::endl;

    std::vector<std::thread> threads2;
    threads2.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
//        threads.emplace_back(&CORRAv1PExp::prepareLocality, this, partition[i], partition[i + 1], deltaNeighbor, xTopoSize, yTopoSize);
        std::thread thread(&CORRAv1PExp::prepareLocality, this, partition[i], partition[i + 1], deltaNeighbor, xTopoSize, yTopoSize);
        threads2.push_back(std::move(thread));
    }
    for (auto &thread : threads2) {
        thread.join();
    }

    std::cout << "Den day" << std::endl;
    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::createLocality, this, partition[i], partition[i + 1], deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        std::cout << "Here " << i << std::endl;
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::createLocalRouting, this, partition[i], partition[i + 1], xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::findBR1, this, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::findBRn, this, partition[i], partition[i + 1], 2);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::broadcastLocalBridge, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::createGlobalTraceMap, this, partition[i], partition[i + 1], topo);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&CORRAv1PExp::updateBlockTable, this, partition[i], partition[i + 1], xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

}

void CORRAv1PExp::createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *corraNode = new CORRANode(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            corraNode->setCentered(true);
        }
        this->corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
    }
}

void CORRAv1PExp::addNearFarNeighbors(int startNodeID, int endNodeID, Graph *graph) {
//    auto *p = &this->corra1NodeList;
    for (int i = startNodeID; i < endNodeID; ++i) {
        for (std::pair<int, std::map<int, float> > source : graph->getAdjList()) {
            for (std::pair<int, float> neighbor : source.second) {
                if (neighbor.second == 1) {
                    CORRANode *corraNode = this->corra1NodeList[neighbor.first];
                    this->corra1NodeList[source.first]->addNearNeighbors(corraNode);
//                    (*p)[source.first]->addNearNeighbors((*p)[neighbor.first]);
//                    this->corra1NodeList[source.first]->addNearNeighbors(this->corra1NodeList[neighbor.first]);
                } else {
                    CORRANode *corraNode = this->corra1NodeList[neighbor.first];
                    this->corra1NodeList[source.first]->addFarNeighbors(corraNode);
//                    (*p)[source.first]->addFarNeighbors((*p)[neighbor.first]);
//                    this->corra1NodeList[source.first]->addFarNeighbors(this->corra1NodeList[neighbor.first]);
                }
            }
        }
    }
}

void CORRAv1PExp::prepareLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->prepareLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void CORRAv1PExp::createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->createLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }
}

void CORRAv1PExp::createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->createLocalRouting(xTopoSize);
    }
}

void CORRAv1PExp::findBR1(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->findBR1();
    }
}

void CORRAv1PExp::findBRn(int startNodeID, int endNodeID, int n) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->findToBRn(n);
    }
}

void CORRAv1PExp::broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }
}

void CORRAv1PExp::createGlobalTraceMap(int startNodeID, int endNodeID, Graph *topo) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->createGlobalTraceMap(topo);
    }
}

void CORRAv1PExp::updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->corra1NodeList[i]->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize, this->corra1NodeList);
    }
}

CORRAv1PExp::~CORRAv1PExp() = default;
