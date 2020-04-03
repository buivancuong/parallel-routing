//
// Created by cuongbv on 03/02/2020.
//


#define INFTY 999999999
#define NODE_LATENCY 10
#define CABLE_LATENCY 1

#include <set>
#include <iostream>
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

void CORRANode::addNearNeighbors(CORRANode *nearNeighbor) {
    this->nearNeighbors.insert(std::pair<int, CORRANode*>(nearNeighbor->getNodeID(), nearNeighbor));
}

void CORRANode::addFarNeighbors(CORRANode *farNeighbor) {
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

void CORRANode::createLocality(int deltaNeighbor,int xBlockSize, int yBlockSize, int xTopoSize) {
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
    int maxGridHop = CORRAUtils::getMaxHopinBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
//    std::cout << "max grid hop " << maxGridHop << std::endl;
    for (int i = deltaNeighbor - 1; i < maxGridHop - 2; ++i) {
//        std::cout << "i " << i << std::endl;
        std::map<int, CORRANode*> higherNeighbors;
        for (std::pair<int, CORRANode*> lowerNeighbor : this->locality[i]) {
            int size = lowerNeighbor.second->getLocality()[0].size();
//            std::cout << "precheck size " << size << std::endl;
            std::map<int, CORRANode*> localityOfLowerNeighbor (lowerNeighbor.second->getLocality()[0]);
            for (std::pair<int, CORRANode*> neighbor : localityOfLowerNeighbor) {
                --size;
                if (size < 0) break;
//                std::cout << "size " << size << std::endl;
//                std::cout << "check new in block " << neighbor.first << " on lower neighbor " << lowerNeighbor.first << std::endl;
//                if (added.count(neighbor.first)) continue;
                if (neighbor.first == this->nodeID) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first)) continue;        // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first))) continue;        // neighbor is existed at inner layer neighbor
                if (!higherNeighbors.count(neighbor.first)){        // if new higher neighbor layer is not include new neighbor
                    // Check block ID of this node and neighbor Node
                    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
                    int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
                    if (thisBlock == neighborBlock) {      // if same block with this node, add to locality
                        higherNeighbors.insert(neighbor);
//                        added.insert(neighbor.first);
//                        std::cout << "add from block " << neighbor.first << std::endl;
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

void CORRANode::findToBRn(int n) {
    if (n != 2) return;      // BR1 was found already in Node::findBR1(); we need start find from BR2
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

void CORRANode::createLocalRouting(int xTopoSize) {
    // trace map to reverse Dijkstra shortest path from each other nodes to this->nodeID
    std::map<int, bool> visited;        // <nodeID, visited>
    std::map<int, CORRANode*> localityList;
    // Add this node and all of locality to localTraceMap
    // Add this node
    std::pair<double, int> this_dist_prev(0, this->nodeID);
    std::pair<int, std::pair<double, int> > thisNode(this->nodeID, this_dist_prev);
    this->localTraceMap.insert(thisNode);
    visited.insert(std::pair<int, bool>(this->nodeID, true));
    localityList.insert(std::pair<int, CORRANode*>(this->nodeID, this));
    // Add each other nodes on this->locality to localTraceMap
    for (int i = 0; i < this->locality.size(); ++i) {
        // std::cout << "locality size " << this->locality.size() << std::endl;
        std::map<int, CORRANode*> temp (this->locality[i]);
        for (std::pair<int, CORRANode*> neighbor : temp) {
            if (i == 0) {       // first layer real neighbor
                std::pair<double, int> dist_prev(CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize), this->nodeID);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->localTraceMap.insert(neighborNode);
            } else {        // non real neighbor
                std::pair<double, int> dist_prev(INFTY, -1);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->localTraceMap.insert(neighborNode);
            }
            visited.insert(std::pair<int, bool>(neighbor.first, false));
            localityList.insert(neighbor);
        }
    }
    // Dijkstra Algorithm to construct localTraceMap and visited state
    // std::cout << "localTraceMap size " << this->localTraceMap.size() << std::endl;
    // std::cout << "locality list size " << localityList.size() << std::endl;
    for (std::pair<int, CORRANode*> node : localityList) {
        // std::cout << "nodeID " << node.first << std::endl;
    }
    for (int i = 0; i < this->localTraceMap.size(); ++i) {
        int currentNodeID = -1;
        // Check if all candidate is INFTY, and find new current Node
        for (std::pair<int, bool> iterator : visited){
            if (!iterator.second) {
                if ((currentNodeID == -1) or (this->localTraceMap[iterator.first].first < this->localTraceMap[currentNodeID].first))
                    currentNodeID = iterator.first;
            }
        }
        if (this->localTraceMap[currentNodeID].first == INFTY) break;
        if (currentNodeID == -1) {
            this->localTraceMap.erase(-1);
            break;
        };
        //Perform a Dijkstra iteration
        visited[currentNodeID] = true;
        // std::cout << "currentID " << currentNodeID << std::endl;
        std::map<int, CORRANode*> temp (localityList[currentNodeID]->getLocality()[0]);
        for (std::pair<int, CORRANode*> neighbor : temp) {
            if (localityList.count(neighbor.first)) {
                if (!visited[neighbor.first]) {
                    int destID = neighbor.first;
                    double distance = CORRAUtils::getGridHop(currentNodeID, destID, xTopoSize);
                    if (this->localTraceMap[currentNodeID].first + distance + NODE_LATENCY < this->localTraceMap[destID].first) {
                        this->localTraceMap[destID].first = this->localTraceMap[currentNodeID].first + distance + NODE_LATENCY;
                        this->localTraceMap[destID].second = currentNodeID;
                    }
                }
            }
        }
    }
//    std::map<int, std::pair<double, int> > tempTraceMap (this->localTraceMap);
    for (std::pair<int, std::pair<double, int> > neighbor : this->localTraceMap) {
        // std::cout << "vao trong " << neighbor.first << std::endl;
        if (neighbor.first != this->nodeID) {
            localityList[neighbor.first]->updateLocalRT(this->nodeID, neighbor.second.second, neighbor.second.first);
        }
    }
}

void CORRANode::updateLocalRT(int destNodeID, int nextNodeID, double latency) {
    std::pair<int, double> nextNode(nextNodeID, latency);
    this->localRT.insert(std::pair<int, std::pair<int, double> >(destNodeID, nextNode));
}

void CORRANode::broadcastLocalBridge(int xBlockSize, int yBlockSize, int xTopoSize) {
    for (const std::map<int, CORRANode*>& localLayer : this->locality) {
        for (std::pair<int, CORRANode*> neighbor : localLayer) {
            for (const std::vector<std::pair<int, CORRANode*> >& bridge : this->ownBridges) {
                neighbor.second->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize);
            }
        }
    }
}

void CORRANode::updateBridgeList(std::vector<std::pair<int, CORRANode *> > bridge, int xBlockSize, int yBlockSize, int xTopoSize) {
    int destBlockID = CORRAUtils::getNodeBlock(bridge.back().first, xBlockSize, yBlockSize, xTopoSize);
    if (this->bridgeList[destBlockID].empty()) {
        this->bridgeList[destBlockID].emplace_back(bridge);
    }
}

std::map<int, std::pair<float, int> > CORRANode::getGlobalTraceMap() {
    return this->globalTraceMap;
}

void CORRANode::createGlobalTraceMap(Graph *globalGraph) {
    if (this->isCenterNode) {
        // std::cout << "Dijkstra " << this->nodeID << std::endl;
        this->globalTraceMap = globalGraph->Dijkstra(this->nodeID);
    }
}

void CORRANode::updateBlockTable(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, std::map<int, CORRANode*> corra1NodeList) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    int numBlock = CORRAUtils::getTotalBlocks(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    for (int blockID = 0; blockID < numBlock; ++blockID) {
        if (blockID == thisBlock) continue;
        if (this->bridgeList[blockID].empty()) {
            // SPR
            int centerDestNode = CORRAUtils::getCenterVertex(blockID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
            std::map<int, std::pair<float, int> > trace = corra1NodeList[centerDestNode]->getGlobalTraceMap();
            int nextNodeID = trace[this->nodeID].first;
            this->blockRT.insert(std::pair<int, int>(blockID, nextNodeID));
            continue;
        }       // else, has bridge
        std::vector<std::pair<int, CORRANode*> > bridge = this->bridgeList[blockID][0];
        int next = this->localRT[bridge[0].first].first;
        this->blockRT.insert(std::pair<int, int>(blockID, next));
    }
}

int CORRANode::getNextNodeID(int destID) {
    if (this->globalTraceMap[destID].second == this->nodeID) {
        return destID;
    }
    int prevID = this->globalTraceMap[destID].second;
    while (prevID != this->nodeID) {
        prevID = this->globalTraceMap[destID].second;
    }
    return prevID;
}

bool CORRANode::getCentered() {
    return this->isCenterNode;
}

void CORRANode::setCentered(bool centered) {
    this->isCenterNode = centered;
}

std::vector<std::vector<std::pair<int, CORRANode *> > > CORRANode::getOwnBridges() {
    return this->ownBridges;
}

std::map<int, std::pair<int, double> > CORRANode::getLocalRT() {
    return this->localRT;
}

std::map<int, int> CORRANode::getBlockRT() {
    return this->blockRT;
}

void CORRANode::updateBlockRT(int destBlockID, int nextNodeID) {
    this->blockRT.insert(std::pair<int, int>(destBlockID, nextNodeID));
}

CORRANode::~CORRANode() = default;
