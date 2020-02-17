//
// Created by cuongbv on 17/02/2020.
//

#include "TZNode.h"

TZNode::TZNode() = default;

TZNode::TZNode(int nodeID) {
    this->nodeID = nodeID;
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

void TZNode::extractClosetLandmark() {
    if (this->isLandmark) return;
    if (TZNode::landmarks->empty()) return;

    this->setClosetLandmark(TZNode::landmarks->begin()->first);
    for (std::pair<int, TZNode*> landmark : *TZNode::landmarks) {
        if (this->traceMap[this->closetLandmark].first > this->traceMap[landmark.first].first) {
            this->setClosetLandmark(landmark.first);
        }
    }
}

void TZNode::extractCluster() {
    this->cluster.clear();
    for (std::pair<int, TZNode*> node : *TZNode::nodeList) {
        if (node.first == this->nodeID) continue;
        if (this->traceMap[node.first].first < node.second->getTraceMap()[node.second->getClosetLandmark()].first) {
            this->cluster.insert(node);
        }
    }
}

std::map<int, std::pair<float, int> > TZNode::getTraceMap() {
    return this->traceMap;
}

TZNode::~TZNode() = default;
