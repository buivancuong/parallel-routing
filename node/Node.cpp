//
// Created by cuongbv on 20/12/2019.
//

#define INFTY 999999999
#define NODE_LATENCY 100
#define CABLE_LATENCY 1

#include <algorithm>
#include <iostream>
#include "Node.h"
#include "../utils/CORRAUtils.h"

Node::Node(int nodeID) {
    this->nodeID = nodeID;
}

void Node::createLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize) {
    std::map<int, Node*> localityMap;
    localityMap.insert(std::pair<int, Node*>(this->nodeID, this));
    for (std::pair<int, Node*> neighbor : this->locality[0]) {
        if (localityMap.find(neighbor.first) == localityMap.end()) {        // neu chua co trong locality List
            localityMap.insert(neighbor);
        }
    }
    // Extract Neighbors area
    for (int radius = 1; radius < deltaNeighbor; ++radius) {
        std::map<int, Node*> higherNeighbors;       // vong hang xom ben ngoai
        std::map<int, Node*> lowerNeighbors = this->locality[radius - 1];      // vong trong, vong truoc do
        for (std::pair<int, Node*> lowerNeighbor : lowerNeighbors) {
            std::map<int, Node*> neighborsOfLowerNeighbor = lowerNeighbor.second->getLocality()[0];     // xet cac Node la neighbor cua neighbor
            for (std::pair<int, Node*> neighbor : neighborsOfLowerNeighbor) {
                if (localityMap.count(neighbor.first)) continue;        // neu da co trong locality list thi bo qua
                // con khong thi add
                int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
                int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
                if (thisBlock != neighborBlock) {       // neu khac block
                    int distance = CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize);
                    if (distance <= deltaNeighbor) {        // nhung van trong khoang deltaNeighbor
//                        std::cout << "distance " << distance << std::endl;
                        localityMap.insert(neighbor);       // thi add
                        higherNeighbors.insert(neighbor);
                        continue;
                    }
                    continue;
                }       // con cung block thi duong nhien add, vi chac chan thuoc locality
                localityMap.insert(neighbor);
                higherNeighbors.insert(neighbor);
            }
        }
        this->locality.push_back(higherNeighbors);
    }
    // Extract Block \ Neighbors area
    int maxGridHop = CORRAUtils::getMaxHopinBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);     // max hop Node co the dat den
    for (int radius = deltaNeighbor; radius < maxGridHop; ++radius) {
        std::map<int, Node*> higherNeighbors;       // vong hang xom ben ngoai
        std::map<int, Node*> lowerNeighbors = this->locality[radius - 1];      // vong trong, vong truoc do
        for (std::pair<int, Node*> lowerNeighbor : lowerNeighbors) {
            std::map<int, Node*> neighborsOfLowerNeighbor = lowerNeighbor.second->getLocality()[0];     // xet cac Node la neighbor cua neighbor
            for (std::pair<int, Node*> neighbor : neighborsOfLowerNeighbor) {
                if (localityMap.count(neighbor.first)) continue;        // neu da co trong locality list thi bo qua
                // con khong thi add
                int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
                int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
                if (thisBlock != neighborBlock) {       // neu khac block
                    int distance = CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize);
                    if (distance <= deltaNeighbor) {        // nhung van trong khoang deltaNeighbor
                        localityMap.insert(neighbor);       // thi add
                        higherNeighbors.insert(neighbor);
                        continue;
                    }
                    continue;
                }       // con cung block thi duong nhien add, vi chac chan thuoc locality
                localityMap.insert(neighbor);
                higherNeighbors.insert(neighbor);
            }
        }
        this->locality.push_back(higherNeighbors);
    }
}

std::vector<std::map<int, Node*> > Node::getLocality() {
    return this->locality;
}

void Node::updateLocalRT(int destNodeID, int nextNodeID, double latency) {
    std::pair<int, double> nextNode(nextNodeID, latency);
    this->localRT.insert(std::pair<int, std::pair<int, double> >(destNodeID, nextNode));
}

void Node::createLocalRouting(int xTopoSize) {
    // trace map to reverse Dijkstra shortest path from each other nodes to this->nodeID
    std::map<int, bool> visited;        // <nodeID, visited>
    std::map<int, Node*> localityList;
    // Add this node and all of locality to localTraceMap
    // Add this node
    std::pair<double, int> this_dist_prev(0, this->nodeID);
    std::pair<int, std::pair<double, int> > thisNode(this->nodeID, this_dist_prev);
    this->traceMap.insert(thisNode);
    visited.insert(std::pair<int, bool>(this->nodeID, true));
    localityList.insert(std::pair<int, Node*>(this->nodeID, this));
    // Add each other nodes on this->locality to localTraceMap
    for (int i = 0; i < this->locality.size(); ++i) {
        // std::cout << "locality size " << this->locality.size() << std::endl;
        std::map<int, Node*> temp (this->locality[i]);
        for (std::pair<int, Node*> neighbor : temp) {
            if (i == 0) {       // first layer real neighbor
                std::pair<double, int> dist_prev(CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize), this->nodeID);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->traceMap.insert(neighborNode);
            } else {        // non real neighbor
                std::pair<double, int> dist_prev(INFTY, -1);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->traceMap.insert(neighborNode);
            }
            visited.insert(std::pair<int, bool>(neighbor.first, false));
            localityList.insert(neighbor);
        }
    }
    // Dijkstra Algorithm to construct localTraceMap and visited state
    // std::cout << "localTraceMap size " << this->traceMap.size() << std::endl;
    // std::cout << "locality list size " << localityList.size() << std::endl;
    for (std::pair<int, Node*> node : localityList) {
        // std::cout << "nodeID " << node.first << std::endl;
    }
    for (int i = 0; i < this->traceMap.size(); ++i) {
        int currentNodeID = -1;
        // Check if all candidate is INFTY, and find new current Node
        for (std::pair<int, bool> iterator : visited){
            if (!iterator.second) {
                if ((currentNodeID == -1) or (this->traceMap[iterator.first].first < this->traceMap[currentNodeID].first))
                    currentNodeID = iterator.first;
            }
        }
        if (this->traceMap[currentNodeID].first == INFTY) break;
        if (currentNodeID == -1) {
            this->traceMap.erase(-1);
            break;
        }
        //Perform a Dijkstra iteration
        visited[currentNodeID] = true;
        // std::cout << "currentID " << currentNodeID << std::endl;
        std::map<int, Node*> temp (localityList[currentNodeID]->getLocality()[0]);
        for (std::pair<int, Node*> neighbor : temp) {
            if (localityList.count(neighbor.first)) {
                if (!visited[neighbor.first]) {
                    int destID = neighbor.first;
                    double distance = CORRAUtils::getGridHop(currentNodeID, destID, xTopoSize);
                    if (this->traceMap[currentNodeID].first + distance + NODE_LATENCY < this->traceMap[destID].first) {
                        this->traceMap[destID].first = this->traceMap[currentNodeID].first + distance + NODE_LATENCY;
                        this->traceMap[destID].second = currentNodeID;
                    }
                }
            }
        }
    }
//    std::map<int, std::pair<double, int> > tempTraceMap (this->localTraceMap);
    for (std::pair<int, std::pair<double, int> > neighbor : this->traceMap) {
        // std::cout << "vao trong " << neighbor.first << std::endl;
        if (neighbor.first != this->nodeID) {
            localityList[neighbor.first]->updateLocalRT(this->nodeID, neighbor.second.second, neighbor.second.first);
        }
    }
}

void Node::findBR1() {
    for (std::pair<int, Node*> bridgePoint : this->farNeighbors) {       // for each last point of per bridge 1 (random link)
        // create a new bridge1 (this bridge is exactly a random link)
        std::vector<std::pair<int, Node*> > bridge1 {std::pair<int, Node*>(this->nodeID, this), std::pair<int, Node*>(bridgePoint)};
        this->ownBridges.push_back(bridge1);         // Add new bridge1 to ownBridges list
    }
}

void Node::findToBRn(int n) {
    if (n < 2) return;      // BR1 was found already in Node::findBR1(); we need start find from BR2
    for (int i = 2; i <= n; ++i) {      // procedure for type of BRi; BR1 was found already in Node::findBR1();
        // Find new bridge BRi via endpoint of BR_i-1, (add random link to the existed bridge to create a new bridge)
        std::vector<std::vector<std::pair<int, Node*> > > temp (this->ownBridges);
        for (std::vector<std::pair<int, Node*> > bridge : temp) {        // for each bridge in ownBridges list
            if (bridge.size() < (i + 1)) {        // finding BRi, the size of lower bridge must be less than i.
                // std::cout << "da vao day " << i << std::endl;
                std::map<int, Node*> tempFar (bridge.back().second->getFarNeighbors());
                for (std::pair<int, Node*> newEndPoint : tempFar) {       // ++ each far neighbor of current last end poind
                    std::vector<std::pair<int, Node*> > newBridge = bridge;      // clone a new bridge
                    newBridge.push_back(newEndPoint);       // add the far neighbor of endpoint of current bridge to new bridge
                    if (Node::checkBridge(newBridge)) {         // check if new bridge is exactly a new bridge
                        this->ownBridges.push_back(newBridge);
                    }
                }
            }
        }
    }
}

void Node::addNearNeighbors(Node *nearNeighbor) {
    this->nearNeighbors.insert(std::pair<int, Node*>(nearNeighbor->getNodeID(), nearNeighbor));
}

int Node::getNodeID() {
    return this->nodeID;
}

std::map<int, Node*> Node::getNearNeighbors() {
    return this->nearNeighbors;
}

std::map<int, Node*> Node::getFarNeighbors() const {
    return this->farNeighbors;
}

void Node::addFarNeighbors(Node *farNeighbor) {
    this->farNeighbors.insert(std::pair<int, Node*>(farNeighbor->getNodeID(), farNeighbor));
}

void Node::prepareLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSze) {
    this->locality.emplace_back();      // create the null (placeholder) first element for this->locality
    // first, add the grid neighbors in this->nearNeighbors to this->locality[0]
    for (std::pair<int, Node*> neighbor : this->nearNeighbors) {
        this->locality[0].insert(neighbor);
        // std::cout << "insert grid neighbor " << neighbor.first << std::endl;
    }
    // last, add the random link neighbors in this->farNeighbors to this->locality[0]
    for (std::pair<int, Node*> neighbor : this->farNeighbors) {
        int gridDistance = CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSze);      // check grid distance between this node and its random link neighbor
        if (gridDistance <= deltaNeighbor) {        // if distance < deltaNeighbor (random link neighbor is inside of deltaNeighbors
            this->locality[0].insert(neighbor);
            // std::cout << "insert far neighbor inside of Neighbors " << neighbor.first << std::endl;
            continue;
        }       // else, check blockID of this->nodeID and random link neighbor nodeID
        int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSze);
        int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSze);
        if (thisBlock == neighborBlock) {       // if both of 2 Node is same block
            this->locality[0].insert(neighbor);
            // std::cout << "insert far neighbor insize of Block " << neighbor.first << std::endl;
            continue;
        }       // else, continue
    }
}

std::map<int, std::vector<std::vector<std::pair<int, Node*> > > > Node::getBridgeList() {
    return this->bridgeList;
}

bool Node::checkBridge(const std::vector<std::pair<int, Node*> >& bridge) {
    std::set<int> bridgeCopy;
    for (const std::pair<int, Node*>& point : bridge) {
        bridgeCopy.insert(point.first);
    }
    return bridgeCopy.size() == bridge.size();
}

std::vector<std::vector<std::pair<int, Node*> > > Node::getOwnBridges() {
    return this->ownBridges;
}

void Node::addOwnBridge(const std::vector<std::pair<int, Node*> >& newOwnBridge) {
    this->ownBridges.push_back(newOwnBridge);
}

void Node::broadcastLocalBridge(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (const std::vector<std::pair<int, Node*> >& bridge : this->ownBridges) {
        this->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            for (const std::vector<std::pair<int, Node*> >& bridge : this->ownBridges) {
                neighbor.second->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
            }
        }
    }
}

void Node::updateBridgeList(std::vector<std::pair<int, Node*> > bridge, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int destBlockID = CORRAUtils::getNodeBlock(bridge.back().first, xBlockSize, yBlockSize, xTopoSize);
    if (this->bridgeList[destBlockID].empty()) {        // if have no bridge to destBlockID
        this->bridgeList[destBlockID].emplace_back(bridge);         // add the first bridge to bridge list
        return;
    }
    // else have 1 bridge to destBlockID, count the cost of the new bridge and the existed bridge (source and dest near center node)
    int existedBridgeCost = Node::getBridgeCost(this->bridgeList[destBlockID].back(), xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    int newBridgeCost = Node::getBridgeCost(bridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    if (newBridgeCost >= existedBridgeCost) return;         // if the new bridge is more expensive than the existed bridge
    this->bridgeList[destBlockID].pop_back();       // else delete the existed bridge and push back the new bridge
    this->bridgeList[destBlockID].emplace_back(bridge);
}

void Node::handleMissingBridge(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    std::map<int, bool> blockList;      // <blockID, isConnected> {true/false}
    bool isMissing = false;
    for (const auto& block : this->bridgeList) {
        blockList.insert(std::pair<int, bool>(block.first, !this->bridgeList[block.first].empty()));
        if ((block.first < thisBlock) and this->bridgeList[block.first].empty()) isMissing = true;
    }

    if (!isMissing){
        // std::cout << "Deo missing " << thisBlock << std::endl;
        return;
    }
    for (int blockID = 0; blockID < thisBlock; ++blockID) {         // explore each block to find not connected blocks
        if (!blockList[blockID]) {     // If non connection between thisBlock and blockID < thisBlock
            for (std::pair<int, bool> connectedBlock : blockList) {        // Query to each other medial connected blocks
                if (connectedBlock.second) {        // using the endpoint of a bridge to that connected block (first bridge)
                    for (std::vector<std::pair<int, Node*> > bridgeToMid : this->bridgeList[connectedBlock.first]) {      // for each bridge to the mid-block
                        if (!bridgeToMid.back().second->getBridgeList()[blockID].empty()) {       // if the endpoint (at the mid-block) has one or more bridge to destBlock
                            std::pair<int, Node*> endPointMid = bridgeToMid.back();      // the endpoint of the bridge to mid
                            for (std::vector<std::pair<int, Node*> > bridgeToDest : endPointMid.second->getBridgeList()[blockID]) {       // for each bridge to dest (from the endpoint at mid)
                                std::vector<std::pair<int, Node*> > fullBridge;      // declare full bridge
                                // fullBridge is just declared, must append full bridgeToMid
                                fullBridge.insert(fullBridge.end(), bridgeToMid.begin(), bridgeToMid.end());        // append bridgeToMid to fullBridge
                                if (endPointMid.first != bridgeToDest[0].first) {       // if must routing from endpoint (of the bridge to mid) to the bridge to destBlock
                                    std::vector<std::pair<int, Node*> > localPath = endPointMid.second->localShortestPath(bridgeToDest[0].first);     // create local Dijkstra shortest path from the endpoint to bridgeToDest
                                    // endpoint of bridgeToMid is same as openpoint of localPath, must appent from localPath.begin() +1 (exclude first element)
                                    fullBridge.insert(fullBridge.end(), localPath.begin() + 1, localPath.end());        // append local routing path between 2 bridge endpoints  to fullBridge
                                    fullBridge.insert(fullBridge.end(), bridgeToDest.begin() + 1, bridgeToDest.end());      // append bridgeToDest to fullBridge
                                } else {        // if bridge to destBlock has startpoint is endpoint of bridgeToMid
                                    fullBridge.insert(fullBridge.end(), bridgeToDest.begin() + 1, bridgeToDest.end());
                                }
                                if (this->checkBridge(fullBridge)) {        // check the true bridge
                                    this->updateBridgeList(fullBridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);      // this Node update fullBridge to own bridgeList
                                    this->broadcastMissingBridge(fullBridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);        // this Node broadcast to each other Node in same block

                                    int centerDestBlock = CORRAUtils::getCenterVertex(blockID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);       // extract nodeID of centerNode at destBlock
                                    std::pair<int, Node*> destCenterNode = this->getCenterNode(xBlockSize, yBlockSize, xTopoSize, yTopoSize);        // extract centerNode at destBlock
                                    destCenterNode.second->updateBridgeList(fullBridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);      //      centerNode at destBlock update fullBridge to own bridgeList
                                    destCenterNode.second->broadcastMissingBridge(fullBridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);        // centerNode at destBlock broadcast fullBridge to each other Nodes in same its block
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

double Node::getBridgeLatency(const std::vector<std::pair<int, Node*> >& bridge, int xTopoSize) {
    double totalLatency = 0;
    for (int i = 0; i < bridge.size() - 1; ++i) {
        double hopLatency = CORRAUtils::getGridHop(bridge[i].first, bridge[i+1].first, xTopoSize);
        totalLatency += (NODE_LATENCY + hopLatency);
    }
    return totalLatency;
}

std::vector<std::pair<int, Node*> > Node::localShortestPath(int destID) {
    std::map<int, Node*> localityList;
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            localityList.insert(neighbor);
        }
    }
    std::vector<std::pair<int, Node*> > resultPath = {std::pair<int, Node*>(destID, localityList[destID])};
    while (resultPath.back().first != this->nodeID) {
        resultPath.emplace_back(this->traceMap[resultPath.back().first].second, localityList[this->traceMap[resultPath.back().first].second]);
    }
    std::reverse(resultPath.begin(), resultPath.end());
    return resultPath;
}

void Node::broadcastMissingBridge(const std::vector<std::pair<int, Node*> >& bridge, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
            int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
            if (thisBlock == neighborBlock) {
                neighbor.second->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
            }
        }
    }
}

std::pair<int, Node*> Node::getCenterNode(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    int centerNodeID = CORRAUtils::getCenterVertex(thisBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            if (neighbor.first == centerNodeID) {
                return neighbor;
            }
        }
    }
}

void Node::updateBlockTable(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    int numBlock = CORRAUtils::getTotalBlocks(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    for (int blockID = 0; blockID < numBlock; ++blockID) {
        // std::cout << "block " << blockID << std::endl;
        if (blockID == thisBlock) continue;     // only update Block Routing Table to each other blocks
        if (this->bridgeList[blockID].empty()) {        // if still has no bridge to blockID (destBlock), must route to centerNode
            int centerNodeID = CORRAUtils::getCenterVertex(thisBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);     // extract nodeID of the centerNode of its blockID
            // std::cout << "center " << centerNodeID << std::endl;
//            std::vector<std::pair<int, Node*> > localPath = this->localShortestPath(centerNodeID);       //      extract local shortest path from this->nodeID to centerNode
//            std::cout << "spr size " << localPath.size() << std::endl;
            int next = this->localRT[centerNodeID].first;
            this->bridgeRT.insert(std::pair<int, int>(blockID, next));        // go forward to centerNode via nextNode
            // std::cout << "van deo co cau " << blockID << std::endl;
            continue;       // continue with next blockID
        }       // else, has bridge to blockID, next code segment will be executed.
//        std::vector<std::vector<std::pair<int, Node*> > > fullBridgeList;      // declare list of full bridge from this->nodeID to destBlockID
//        std::cout << "so cau " << this->bridgeList[blockID].size() << std::endl;
//        std::vector<std::vector<std::pair<int, Node*> > > temp (this->bridgeList[blockID]);
//        for (std::vector<std::pair<int, Node*> > bridge : temp) {       // for each bridge in bridge list to destBlockID
//            std::vector<std::pair<int, Node*> > fullBridge;      // declare a full bridge from this->nodeID to destBlockID
//            std::vector<std::pair<int, Node*> > localPath = this->localShortestPath(bridge.begin()->first);      // first, extract local shortest path from this->nodeID to beginpoint of the bridge
//            fullBridge.insert(fullBridge.end(), localPath.begin(), localPath.end());        // append the local shortest path follow fullBridge
//            fullBridge.insert(fullBridge.end(), bridge.begin() + 1, bridge.end());      // second, append the bridge follow to fullBridge (from iter +1 of bridge)
//            fullBridgeList.push_back(fullBridge);       // append the found fullBridge to fullBridgeList
//        }
//        std::vector<std::pair<int, Node*> > bestBridge = fullBridgeList[0];      // find the best bridge for route to destination blockID
//        for (const std::vector<std::pair<int, Node*> >& bridge : fullBridgeList) {
////            std::cout << "Den day " << blockID << std::endl;
//            if (bridge.size() < bestBridge.size()) {        // best bridge is the bridge that has smallest size
//                bestBridge = bridge;
//            }
//        }
//        this->bridgeRT.insert(std::pair<int, int>(blockID, bestBridge[0].first));

        std::vector<std::pair<int, Node*> > firstBridge = this->bridgeList[blockID].front();
        // std::cout << "bridge size " << firstBridge.size() << std::endl;
        int next = this->localRT[firstBridge.front().first].first;
        this->bridgeRT.insert(std::pair<int, int>(blockID, next));
   }
}

void Node::setCentered(bool centered) {
    this->isCenterNode = centered;
}

bool Node::getCentered() {
    return this->isCenterNode;
}

std::map<int, std::pair<int, double> > Node::getLocalRT() {
    return this->localRT;
}

std::map<int, int> Node::getBlockRT() {
    return this->bridgeRT;
}

void Node::updateBlockRT(int destBlockID, int nextNodeID) {
    this->bridgeRT.insert(std::pair<int, int>(destBlockID, nextNodeID));
}

int Node::getBridgeCost(std::vector<std::pair<int, Node *> > bridge, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    std::pair<int, Node*> sourceBridge = bridge.front();
    int sourceBlockID = CORRAUtils::getNodeBlock(sourceBridge.first, xBlockSize, yBlockSize, xTopoSize);
    int sourceCenterNodeID = CORRAUtils::getCenterVertex(sourceBlockID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    int sourceCost = CORRAUtils::getGridHop(sourceBridge.first, sourceCenterNodeID, xTopoSize);

    std::pair<int, Node*> destBridge = bridge.back();
    int destBlockID = CORRAUtils::getNodeBlock(destBridge.first, xBlockSize, yBlockSize, xTopoSize);
    int destCenterNodeID = CORRAUtils::getCenterVertex(destBlockID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    int destCost = CORRAUtils::getGridHop(destBridge.first, destCenterNodeID, xTopoSize);

    int totalCost = (int)(sourceCost + destCost + bridge.size() - 2);
    return totalCost;
}

Node::Node() = default;


Node::~Node() = default;
