//
// Created by cuongbv on 19/12/2019.
//

#include "CORRADistributed.h"


CORRADistributed::CORRADistributed() = default;

CORRADistributed::~CORRADistributed() = default;

void CORRADistributed::preparingRouting(Graph graph, int sourceID) {
    this->createLocality(graph, sourceID);
    this->findAllBridges(graph, sourceID);
    this->updateBridgeTable(graph, sourceID);
}

void CORRADistributed::createLocality(Graph graph, int sourceID) {
    
}

void CORRADistributed::findAllBridges(Graph graph, int sourceID) {

}

void CORRADistributed::updateBridgeTable(Graph graph, int sourceID) {

}

