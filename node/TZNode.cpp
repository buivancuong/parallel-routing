//
// Created by cuongbv on 17/02/2020.
//

#include <iostream>
#include "TZNode.h"

TZNode::TZNode() = default;

TZNode::TZNode(int nodeID) {
    this->nodeID = nodeID;
    this->isLandmark = false;
}

void TZNode::setIsLandMark() {
    this->isLandmark = true;
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

bool TZNode::isLandmarkNode() {
    return this->isLandmark;
}

void TZNode::createTraceMap(Graph *globalGraph) {
    // std::cout << "create Dijktra " << this->nodeID << std::endl;
    this->traceMap = globalGraph->Dijkstra(this->nodeID);
}

void TZNode::extractClosetLandmark(std::map<int, TZNode*> landmarks) {
    if (this->isLandmark) return;

    this->setClosetLandmark(landmarks.begin()->first);
    for (std::pair<int, TZNode*> landmark : landmarks) {
        if (this->traceMap[this->closetLandmark].first > this->traceMap[landmark.first].first) {
            // std::cout << "closet landmark of " << this->nodeID << " is " << landmark.first << std::endl;
            this->setClosetLandmark(landmark.first);
        }
    }
}

void TZNode::extractCluster(const std::map<int, TZNode*>& nodeList) {
    this->cluster.clear();
    for (std::pair<int, TZNode*> node : nodeList) {
        if (node.first == this->nodeID) continue;
        if (this->traceMap[node.first].first < node.second->getTraceMap()[node.second->getClosetLandmark()].first) {
            // std::cout << "cluster of " << this->nodeID << " chua them " << node.first << std::endl;
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

TZNode::~TZNode() = default;
