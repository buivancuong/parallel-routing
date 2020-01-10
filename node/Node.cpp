//
// Created by cuongbv on 20/12/2019.
//

#define INFTY 999999999
#define NODE_LATENCY 10
#define CABLE_LATENCY 1

#include <algorithm>
#include <iostream>
#include "Node.h"
#include "../utils/CORRAUtils.h"

Node::Node(int nodeID) {
    this->nodeID = nodeID;
}

void Node::createLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    // Discovery Neighbors
    std::cout << "Discover Neighbors" << std::endl;
    for (int i = 0; i < deltaNeighbor - 1; ++i) {       // Counting from 0, not 1
        std::map<int, Node*> higherNeighbors;
        for (std::pair<int, Node*> lowerNeighbor : this->locality[i]) {      // for each vertex on lower neighbor layer (current neighbor)
            for (const std::pair<int, Node*> neighbor : lowerNeighbor.second->getLocality()[0]) {       // for each neighbor of each above vertice (new neighbor)
                if (neighbor.first == this->nodeID) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first)) continue;        // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first))) continue;        // neighbor is existed at inner layer neighbor
                if (!higherNeighbors.count(neighbor.first)){        // if new higher neighbor layer is not include new neighbor
                    if (CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize) <= deltaNeighbor) {
                        higherNeighbors.insert(neighbor);       // add new neighbor into new higher layer neighbor
                        std::cout << "add neighbor " << neighbor.first << std::endl;
                    }
                }
            }
        }
        this->locality.push_back(higherNeighbors);
    }
    // Discover Block \sub Neighbors
    std::cout << "Discover Block sub Neighbor" << std::endl;
    // Extract max grid-hop for the left Node in the Block
    int maxGridHop = CORRAUtils::getMaxHopinBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    for (int i = deltaNeighbor - 1; i < maxGridHop; ++i) {
        std::map<int, Node*> higherNeighbors;
        for (std::pair<int, Node*> lowerNeighbor : this->locality[i]) {
            for (std::pair<int, Node*> neighbor : lowerNeighbor.second->getLocality()[0]) {
                if (neighbor.first == this->nodeID) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first)) continue;        // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first))) continue;        // neighbor is existed at inner layer neighbor
                if (!higherNeighbors.count(neighbor.first)){        // if new higher neighbor layer is not include new neighbor
                    // Check block ID of this node and neighbor Node
                    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
                    int neighborBloclk = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
                    if (thisBlock == neighborBloclk) {      // if same block with this node, add to locality
                        higherNeighbors.insert(neighbor);
                        std::cout << "add block " << neighbor.first << std::endl;
                    }
                }
            }
        }
        this->locality.push_back(higherNeighbors);
    }
    std::cout << "Done nodeID " << this->nodeID << std::endl;
}

std::vector<std::map<int, Node*> > Node::getLocality() {
    return this->locality;
}

void Node::updateLocalRT(int destNodeID, int nextNodeID, double latency) {
    std::pair<int, double> nextNode(nextNodeID, latency);
    this->localRT.insert(std::pair<int, std::pair<int, double> >(destNodeID, nextNode));
}

void Node::createLocalRouting(int xTopoSize, int yTopoSize) {
    // trace map to reverse Dijkstra shortest path from each other nodes to this->nodeID
    std::map<int, bool> visited;        // <nodeID, visited>
    std::map<int, Node*> localityList;
    // Add this node and all of locality to traceMap
    // Add this node
    std::pair<double, int> this_dist_prev(0, this->nodeID);
    std::pair<int, std::pair<double, int> > thisNode(this->nodeID, this_dist_prev);
    this->traceMap.insert(thisNode);
    visited.insert(std::pair<int, bool>(this->nodeID, true));
    localityList.insert(std::pair<int, Node*>(this->nodeID, this));
    // Add each other nodes on this->locality to traceMap
    for (int i = 0; i < this->locality.size(); ++i) {
        for (std::pair<int, Node*> neighbor : this->locality[i]) {
            if (i == 0) {       // first layer real neighbor
                std::pair<double, int> dist_prev(CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSize), this->nodeID);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->traceMap.insert(neighborNode);
                visited.insert(std::pair<int, bool>(neighbor.first, false));
                localityList.insert(neighbor);
            } else {        // non real neighbor
                std::pair<double, int> dist_prev(INFTY, -1);
                std::pair<int, std::pair<double, int> > neighborNode(neighbor.first, dist_prev);
                this->traceMap.insert(neighborNode);
                visited.insert(std::pair<int, bool>(neighbor.first, false));
                localityList.insert(neighbor);
            }
        }
    }
    // Dijkstra Algorithm to construct traceMap and visited state
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
        //Perform a Dijkstra iteration
        visited[currentNodeID] = true;
        for (const std::pair<int, Node*> neighbor : localityList[currentNodeID]->getLocality()[0]) {
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
    for (std::pair<int, std::pair<double, int> > neighbor : this->traceMap) {
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
        for (const std::vector<std::pair<int, Node*> >& bridge : this->ownBridges) {        // for each bridge in ownBridges list
            if (bridge.size() < i) {        // finding BRi, the size of lower bridge must be less than i.
                for (std::pair<int, Node*> newEndPoint : bridge.back().second->getFarNeighbors()) {       // ++ each far neighbor of current last end poind
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

void Node::prepareLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSze, int yTopoSize) {
    this->locality.emplace_back();      // create the null (placeholder) first element for this->locality
    // first, add the grid neighbors in this->nearNeighbors to this->locality[0]
    for (std::pair<int, Node*> neighbor : this->nearNeighbors) {
        this->locality[0].insert(neighbor);
        std::cout << "insert grid neighbor " << neighbor.first << std::endl;
    }
    // last, add the random link neighbors in this->farNeighbors to this->locality[0]
    for (std::pair<int, Node*> neighbor : this->farNeighbors) {
        int gridDistance = CORRAUtils::getGridHop(this->nodeID, neighbor.first, xTopoSze);      // check grid distance between this node and its random link neighbor
        if (gridDistance <= deltaNeighbor) {        // if distance < deltaNeighbor (random link neighbor is inside of deltaNeighbors
            this->locality[0].insert(neighbor);
            std::cout << "insert far neighbor inside of Neighbors " << neighbor.first << std::endl;
            continue;
        }       // else, check blockID of this->nodeID and random link neighbor nodeID
        int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSze);
        int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSze);
        if (thisBlock == neighborBlock) {       // if both of 2 Node is same block
            this->locality[0].insert(neighbor);
            std::cout << "insert far neighbor insize of Block " << neighbor.first << std::endl;
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

void Node::broadcastLocalBridge(int xBlockSize, int yBlockSize, int xTopoSize) {
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            for (const std::vector<std::pair<int, Node*> >& bridge : this->ownBridges) {
                neighbor.second->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize);
            }
        }
    }
}

void Node::updateBridgeList(std::vector<std::pair<int, Node*> > bridge, int xBlockSize, int yBlockSize, int xTopoSize) {
    int destBlockID = CORRAUtils::getNodeBlock(bridge.back().first, xBlockSize, yBlockSize, xTopoSize);
    this->bridgeList[destBlockID].push_back(bridge);
}

void Node::handleMissingBridge(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    std::map<int, bool> blockList;      // <blockID, isConnected> {true/false}
    bool isMissing = false;
    for (const auto& block : this->bridgeList) {
        blockList.insert(std::pair<int, bool>(block.first, !this->bridgeList[block.first].empty()));
        if ((block.first < thisBlock) and this->bridgeList[block.first].empty()) isMissing = true;
    }

    if (!isMissing) return;
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
                                    this->updateBridgeList(fullBridge, xBlockSize, yBlockSize, xTopoSize);      // this Node update fullBridge to own bridgeList
                                    this->broadcastMissingBridge(fullBridge, xBlockSize, yBlockSize, xTopoSize);        // this Node broadcast to each other Node in same block

                                    int centerDestBlock = CORRAUtils::getCenterVertex(blockID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);       // extract nodeID of centerNode at destBlock
                                    std::pair<int, Node*> destCenterNode = this->getCenterNode(xBlockSize, yBlockSize, xTopoSize, yTopoSize);        // extract centerNode at destBlock
                                    destCenterNode.second->updateBridgeList(fullBridge, xBlockSize, yBlockSize, xTopoSize);      //      centerNode at destBlock update fullBridge to own bridgeList
                                    destCenterNode.second->broadcastMissingBridge(fullBridge, xBlockSize, yBlockSize, xTopoSize);        // centerNode at destBlock broadcast fullBridge to each other Nodes in same its block
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

void Node::broadcastMissingBridge(const std::vector<std::pair<int, Node*> >& bridge, int xBlockSize, int yBlockSize, int xTopoSize) {
    for (const std::map<int, Node*>& localLayer : this->locality) {
        for (std::pair<int, Node*> neighbor : localLayer) {
            int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
            int neighborBlock = CORRAUtils::getNodeBlock(neighbor.first, xBlockSize, yBlockSize, xTopoSize);
            if (thisBlock == neighborBlock) {
                neighbor.second->updateBridgeList(bridge, xBlockSize, yBlockSize, xTopoSize);
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

void Node::updateBlockTable(int destBlockID, int nextNodeID, double latency, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int thisBlock = CORRAUtils::getNodeBlock(this->nodeID, xBlockSize, yBlockSize, xTopoSize);
    for (int blockID = 0; blockID < this->bridgeList.size(); ++blockID) {
        if (blockID == thisBlock) continue;     // only update Block Routing Table to each other blocks
        if (this->bridgeList[blockID].empty()) {        // if still has no bridge to blockID (destBlock), must route to centerNode
            int centerNodeID = CORRAUtils::getCenterVertex(this->nodeID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);     // extract nodeID of the centerNode of its blockID
            std::vector<std::pair<int, Node*> > localPath = this->localShortestPath(centerNodeID);       //      extract local shortest path from this->nodeID to centerNode
            this->bridgeRT.insert(std::pair<int, int>(blockID, localPath[0].first));        // go forward to centerNode via nextNode
            continue;       // continue with next blockID
        }       // else, has bridge to blockID, next code segment will be executed.
        std::vector<std::vector<std::pair<int, Node*> > > fullBridgeList;      // declare list of full bridge from this->nodeID to destBlockID
        for (std::vector<std::pair<int, Node*> > bridge : this->bridgeList[blockID]) {       // for each bridge in bridge list to destBlockID
            std::vector<std::pair<int, Node*> > fullBridge;      // declare a full bridge from this->nodeID to destBlockID
            std::vector<std::pair<int, Node*> > localPath = this->localShortestPath(bridge.begin()->first);      // first, extract local shortest path from this->nodeID to beginpoint of the bridge
            fullBridge.insert(fullBridge.end(), localPath.begin(), localPath.end());        // append the local shortest path follow fullBridge
            fullBridge.insert(fullBridge.end(), bridge.begin() + 1, bridge.end());      // second, append the bridge follow to fullBridge (from iter +1 of bridge)
            fullBridgeList.push_back(fullBridge);       // append the found fullBridge to fullBridgeList
        }
        std::vector<std::pair<int, Node*> > bestBridge = fullBridgeList[0];      // find the best bridge for route to destination blockID
        for (const std::vector<std::pair<int, Node*> >& bridge : fullBridgeList) {
            if (bridge.size() < bestBridge.size()) {        // best bridge is the bridge that has smallest size 
                bestBridge = bridge;
            }
        }
        this->bridgeRT.insert(std::pair<int, int>(blockID, bestBridge[0].first));
    }
}

Node::Node() = default;


Node::~Node() = default;
