//
// Created by cuongbv on 28/02/2020.
//

#include <vector>
#include <cmath>
#include <iostream>
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/CORRANode.h"
#include "../../utils/CORRAUtils.h"

std::map<int, CORRANode*> corra1NodeList;
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

    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);

    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        auto *corraNode = new CORRANode(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            corraNode->setCentered(true);
        }
        corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
    }

    // Add near neighbors (grid neighbors) and far neighbors (random link neighbors)
    for (std::pair<int, std::map<int, float> > sourceNode : smallWorld2DGrid->getAdjList()) {
        for (std::pair<int, float> neighbor: sourceNode.second) {
            if (neighbor.second == 1) {
                corra1NodeList[sourceNode.first]->addNearNeighbors(corra1NodeList[neighbor.first]);
            } else {
                corra1NodeList[sourceNode.first]->addFarNeighbors(corra1NodeList[neighbor.first]);
            }
        }
    }

    // Prepare the locality, add near and far neighbors to the locality
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->prepareLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }

    // Create the locality fully from the 1-hop neighbors
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->createLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }

    // Create the routing table on the locality
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->createLocalRouting(xTopoSize);
    }

    // Find all bridges 1 from 1 node
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->findBR1();
    }

    // Find all bridges 2 from 1 node
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->findToBRn(2);
    }

    // Broadcast bridges to overwhelmed the locality
    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }

    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        corraNode.second->createGlobalTraceMap(smallWorld2DGrid);
    }

    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
        std::cout << "update node " << corraNode.first << std::endl;
        corraNode.second->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize, corra1NodeList);
    }

    return 0;
}