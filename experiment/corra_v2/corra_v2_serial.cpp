//
// Created by cuongbv on 29/02/2020.
//

#include <cmath>
#include <chrono>
#include <iostream>
#include <fstream>
#include "../../node/Node.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/CORRAUtils.h"


int main() {
    std::map<int, Node*> corra2NodeList;

    int xTopoSize = 64;
    int yTopoSize = 64;
    int deltaNeighbor = 6;
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

    auto begin = std::chrono::system_clock::now();
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    auto doneTopo = std::chrono::system_clock::now();

    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        Node *node = new Node(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
        }
        corra2NodeList.insert(std::pair<int, Node*>(i, node));
    }

    for (std::pair<int, std::map<int, float> > sourceNode : smallWorld2DGrid->getAdjList()) {
        for (std::pair<int, float> neighbor: sourceNode.second) {
            if (neighbor.second == 1) {
                corra2NodeList[sourceNode.first]->addNearNeighbors(corra2NodeList[neighbor.first]);
            } else {
                corra2NodeList[sourceNode.first]->addFarNeighbors(corra2NodeList[neighbor.first]);
            }
        }
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->createLocalRouting(xTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->findBR1();
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->findToBRn(4);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
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