//
// Created by cuongbv on 21/02/2020.
//

#include "CORRAv2SExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/Node.h"
#include "../../utils/CORRAUtils.h"

CORRAv2SExp::CORRAv2SExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float> &alphas) {
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, Node*> corra2NodeList;

    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        Node *node = new Node(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
        }
        corra2NodeList.insert(std::pair<int, Node*>(i, node));
    }

    for (std::pair<int, std::map<int, float> > sourceNode : topo->getAdjList()) {
        for (std::pair<int, float> neighbor: sourceNode.second) {
            if (neighbor.second == 1) {
                corra2NodeList[sourceNode.first]->addNearNeighbors(corra2NodeList[neighbor.first]);
            } else {
                corra2NodeList[sourceNode.first]->addFarNeighbors(corra2NodeList[neighbor.first]);
            }
        }
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }

    for (std::pair<int, Node*> corraNode : corra2NodeList) {
        corraNode.second->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
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
}

CORRAv2SExp::~CORRAv2SExp() = default;
