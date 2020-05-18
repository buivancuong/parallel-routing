//
// Created by cuongbv on 17/02/2020.
//

#include <iostream>
#include "TZNode.h"

TZNode::TZNode() = default;

TZNode::TZNode(int nodeID) {
    this->nodeID = nodeID;
<<<<<<< HEAD
    this->isLandmark = false;
}

void TZNode::setIsLandMark() {
    this->isLandmark = true;
=======
>>>>>>> graphlib
}

void TZNode::setClosetLandmark(int landmarkNodeID) {
    this->closetLandmark = landmarkNodeID;
}

int TZNode::getNodeID() {
    return this->nodeID;
}

int TZNode::getClosetLandmark() {
    return this->closetLandmark;
}

<<<<<<< HEAD
bool TZNode::isLandmarkNode() {
    return this->isLandmark;
}

void TZNode::createTraceMap(Graph *globalGraph) {
    std::cout << "create Dijktra " << this->nodeID << std::endl;
=======
void TZNode::createTraceMap(Graph *globalGraph) {
    // std::cout << "create Dijktra " << this->nodeID << std::endl;
>>>>>>> graphlib
    this->traceMap = globalGraph->Dijkstra(this->nodeID);
}

void TZNode::extractClosetLandmark(std::map<int, TZNode*> landmarks) {
<<<<<<< HEAD
    if (this->isLandmark) return;
=======
    if (this->closetLandmark == this->nodeID) return;
>>>>>>> graphlib

    this->setClosetLandmark(landmarks.begin()->first);
    for (std::pair<int, TZNode*> landmark : landmarks) {
        if (this->traceMap[this->closetLandmark].first > this->traceMap[landmark.first].first) {
<<<<<<< HEAD
            std::cout << "closet landmark of " << this->nodeID << " is " << landmark.first << std::endl;
=======
            // std::cout << "closet landmark of " << this->nodeID << " is " << landmark.first << std::endl;
>>>>>>> graphlib
            this->setClosetLandmark(landmark.first);
        }
    }
}

void TZNode::extractCluster(const std::map<int, TZNode*>& nodeList) {
    this->cluster.clear();
    for (std::pair<int, TZNode*> node : nodeList) {
        if (node.first == this->nodeID) continue;
        if (this->traceMap[node.first].first < node.second->getTraceMap()[node.second->getClosetLandmark()].first) {
<<<<<<< HEAD
            std::cout << "cluster of " << this->nodeID << " chua them " << node.first << std::endl;
=======
            // std::cout << "cluster of " << this->nodeID << " chua them " << node.first << std::endl;
>>>>>>> graphlib
            this->cluster.insert(node);
        }
    }
}

std::map<int, std::pair<float, int> > TZNode::getTraceMap() {
    return this->traceMap;
}

std::map<int, TZNode*> TZNode::getCluster() {
    return this->cluster;
}

<<<<<<< HEAD
=======
void TZNode::createClusterRT() {
    for (std::pair<int, TZNode*> nearNode : this->cluster) {
        int nextNodeID = this->traceMap[nearNode.first].second;
        while (nextNodeID != this->nodeID) {
            nextNodeID = this->traceMap[nextNodeID].second;
        }
        this->clusterRT.insert(std::pair<int, int>(nearNode.first, nextNodeID));
    }
}

void TZNode::createLandmarkRT(const std::map<int, TZNode*>& landmarkSet) {
    for (std::pair<int, TZNode*> landmark : landmarkSet) {
        int nextNodeID = this->traceMap[landmark.first].second;
        while (nextNodeID != this->nodeID) {
            nextNodeID = this->traceMap[nextNodeID].second;
        }
        this->landmarkRT.insert(std::pair<int, int>(landmark.first, nextNodeID));
    }
}

std::map<int, int> TZNode::getClusterRT() {
    return this->clusterRT;
}

std::map<int, int> TZNode::getLandmarkRT() {
    return this->landmarkRT;
}

void TZNode::updateClusterRT(int destNodeID, int nextNodeID) {
    this->clusterRT.insert(std::pair<int, int>(destNodeID, nextNodeID));
}

void TZNode::updateLandmarkRT(int destLandmarkNodeID, int nextNodeID) {
    this->landmarkRT.insert(std::pair<int, int>(destLandmarkNodeID, nextNodeID));
}

>>>>>>> graphlib
TZNode::~TZNode() = default;
