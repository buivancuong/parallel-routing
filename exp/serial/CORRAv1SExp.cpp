//
// Created by cuongbv on 20/02/2020.
//

#include <iostream>
#include "CORRAv1SExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/CORRANode.h"
#include "../../utils/CORRAUtils.h"

CORRAv1SExp::CORRAv1SExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float>& alphas) {
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, CORRANode*> corra1NodeList;

    // Create list of Nodes on Topo
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
    for (std::pair<int, std::map<int, float> > sourceNode : topo->getAdjList()) {
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
        corraNode.second->createGlobalTraceMap(topo);
    }

    for (std::pair<int, CORRANode*> corraNode : corra1NodeList) {
<<<<<<< HEAD
        std::cout << "update node " << corraNode.first << std::endl;
=======
        // std::cout << "update node " << corraNode.first << std::endl;
>>>>>>> graphlib
        corraNode.second->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize, corra1NodeList);
    }
}

CORRAv1SExp::~CORRAv1SExp() = default;
