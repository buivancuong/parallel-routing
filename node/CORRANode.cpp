//
// Created by cuongbv on 03/02/2020.
//

#include <set>
#include "CORRANode.h"
#include "../utils/CORRAUtils.h"

CORRANode::CORRANode() = default;

CORRANode::CORRANode(int nodeID) {
    this->nodeID = nodeID;
}

bool CORRANode::checkBridge(const std::vector<std::pair<int, CORRANode *> > &bridge) {
    std::set<int> bridgeCopy;
    for (const std::pair<int, CORRANode*>& point : bridge) {
        bridgeCopy.insert(point.first);
    }
    return bridgeCopy.size() == bridge.size();
}

std::vector<std::map<int, CORRANode *> > CORRANode::getLocality() {
    return this->locality;
}

std::map<int, std::vector<std::vector<std::pair<int, CORRANode *> > > > CORRANode::getBridgeList() {
    return this->bridgeList;
}

int CORRANode::getNodeID() {
    return this->nodeID;
}

std::map<int, CORRANode *> CORRANode::getNearNeighbors() {
    return this->nearNeighbors;
}

std::map<int, CORRANode *> CORRANode::getFarNeighbors() {
    return this->farNeighbors;
}

void CORRANode::addNearNeighbor(CORRANode *nearNeighbor) {
    this->nearNeighbors.insert(std::pair<int, CORRANode*>(nearNeighbor->getNodeID(), nearNeighbor));
}

void CORRANode::addFarNeighbor(CORRANode *farNeighbor) {
    this->farNeighbors.insert(std::pair<int, CORRANode*>(farNeighbor->getNodeID(), farNeighbor));
}

void CORRANode::prepareLocality(int deltaNeighbor, int xTopoSize, int yTopoSize) {
    this->locality.emplace_back();
    // first, add the grid neighbors in this->nearNeighbors to this->locality[0]
    for (std::pair<int, CORRANode*> neighbor : this->nearNeighbors) {
        this->locality[0].insert(neighbor);
    }
    // last, add the random link neighbors in this->farNeighbors to this->locality[0]
    for (std::pair<int, CORRANode*> neighbor : this->farNeighbors) {
        int gridDistance = CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize);
        if (gridDistance <= deltaNeighbor) {
            this->locality[0].insert(neighbor);
            continue;
        }
    }

}

void CORRANode::createLocality(int deltaNeighbor, int xTopoSize, int yTopoSize) {
    // Discovery Neighbors
    for (int i = 0; i < deltaNeighbor - 1; ++i) {       // Counting from 0, not 1
//        std::cout << "i " << i << std::endl;
        std::map<int, CORRANode*> higherNeighbors;
        for (std::pair<int, CORRANode*> lowerNeighbor : this->locality[i]) {      // for each vertex on lower neighbor layer (current neighbor)
            int size = lowerNeighbor.second->getLocality()[0].size();
//            std::cout << "presize " << size << std::endl;
            std::map<int, CORRANode*> localityOfLowerNeighbor (lowerNeighbor.second->getLocality()[0]);
            for (std::pair<int, CORRANode*> neighbor : localityOfLowerNeighbor) {       // for each neighbor of each above vertex (new neighbor)
                --size;
                if (size < 0) break;
//                std::cout << "size " << size << std::endl;
//                std::cout << "check new in neighbor " << neighbor.first << " on lower neighbor " << lowerNeighbor.first << std::endl;
//                if (added.count(neighbor.first)) continue;
                if (this->locality[i].count(neighbor.first)) continue;      // 2 nodes in same layer of locality is connected
                if (neighbor.first == this->nodeID) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first)) continue;        // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first))) continue;        // neighbor is existed at inner layer neighbor
                if (!higherNeighbors.count(neighbor.first)){        // if new higher neighbor layer is not include new neighbor
                    if (CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize) <= deltaNeighbor) {
                        higherNeighbors.insert(neighbor);       // add new neighbor into new higher layer neighbor
//                        std::cout << "add from neighbor " << neighbor.first << std::endl;
//                        added.insert(neighbor.first);
                    }
                }
            }
        }
        this->locality.push_back(higherNeighbors);
    }
}

void CORRANode::findBR1() {
    for (std::pair<int, CORRANode*> bridgePoint : this->farNeighbors) {       // for each last point of per bridge 1 (random link)
        // create a new bridge1 (this bridge is exactly a random link)
        std::vector<std::pair<int, CORRANode*> > bridge1 {std::pair<int, CORRANode*>(this->nodeID, this), std::pair<int, CORRANode*>(bridgePoint)};
        this->ownBridges.push_back(bridge1);         // Add new bridge1 to ownBridges list
    }
}

void CORRANode::findBRn(int n) {
    if (n < 2) return;      // BR1 was found already in Node::findBR1(); we need start find from BR2
    for (int i = 2; i <= n; ++i) {      // procedure for type of BRi; BR1 was found already in Node::findBR1();
        // Find new bridge BRi via endpoint of BR_i-1, (add random link to the existed bridge to create a new bridge)
        std::vector<std::vector<std::pair<int, CORRANode*> > > temp (this->ownBridges);
        for (std::vector<std::pair<int, CORRANode*> > bridge : temp) {        // for each bridge in ownBridges list
            if (bridge.size() < (i + 1)) {        // finding BRi, the size of lower bridge must be less than i.
                std::map<int, CORRANode*> tempFar (bridge.back().second->getFarNeighbors());
                for (std::pair<int, CORRANode*> newEndPoint : tempFar) {       // ++ each far neighbor of current last end poind
                    std::vector<std::pair<int, CORRANode*> > newBridge = bridge;      // clone a new bridge
                    newBridge.push_back(newEndPoint);       // add the far neighbor of endpoint of current bridge to new bridge
                    if (CORRANode::checkBridge(newBridge)) {         // check if new bridge is exactly a new bridge
                        this->ownBridges.push_back(newBridge);
                    }
                }
            }
        }
    }
}

CORRANode::~CORRANode() = default;
