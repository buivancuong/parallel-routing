//
// Created by cuongbv on 17/02/2020.
//

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
    this->traceMap = globalGraph->Dijkstra(this->nodeID);
}

void TZNode::extractClosetLandmark(std::map<int, TZNode*> landmarks) {
    if (this->isLandmark) return;

    this->setClosetLandmark(landmarks.begin()->first);
    for (std::pair<int, TZNode*> landmark : landmarks) {
        if (this->traceMap[this->closetLandmark].first > this->traceMap[landmark.first].first) {
            this->setClosetLandmark(landmark.first);
        }
    }
}

void TZNode::extractCluster(std::map<int, TZNode*> nodeList) {
    this->cluster.clear();
    for (std::pair<int, TZNode*> node : nodeList) {
        if (node.first == this->nodeID) continue;
        if (this->traceMap[node.first].first < node.second->getTraceMap()[node.second->getClosetLandmark()].first) {
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

TZNode::~TZNode() = default;
