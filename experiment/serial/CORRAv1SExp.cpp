//
// Created by cuongbv on 20/02/2020.
//

#include "CORRAv1SExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/CORRANode.h"

CORRAv1SExp::CORRAv1SExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float>& alphas) {
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, CORRANode*> corraNodeList;

    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        auto *corraNode = new CORRANode(i);
        corraNodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
    }

    for (std::pair<int, std::map<int, float> > sourceNode : topo->getAdjList()) {
        for (std::pair<int, float> neighbor: sourceNode.second) {
            if (neighbor.second == 1) {
                corraNodeList[sourceNode.first]->addNearNeighbor(corraNodeList[neighbor.first]);
            } else {
                corraNodeList[sourceNode.first]->addFarNeighbor(corraNodeList[neighbor.first]);
            }
        }
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->prepareLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->createLocality(deltaNeighbor, xTopoSize, yTopoSize);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->createLocalRouting(xTopoSize);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->findBR1();
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->findBRn(2);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->createGlobalTraceMap(topo);
    }

    for (std::pair<int, CORRANode*> corraNode : corraNodeList) {
        corraNode.second->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

CORRAv1SExp::~CORRAv1SExp() = default;
