//
// Created by cuongbv on 23/01/2020.
//

#include <iostream>
#include "FatTree.h"

FatTree::~FatTree() = default;

FatTree::FatTree(int k) : Graph() {
    if (k % 2 != 0) {
        std::cout << "k must be even" << std::endl;
        return;
    }
//    if (k > 256) {
//        std::cout << "k must be less than  256" << std::endl;
//        return;
//    }
    // constructs parameters of Fat tree
    this->paramK = k;
    this->numCoreSwitch = k * k / 4;
    this->numPods = k;
    this->numSwitchPerPod = k;
    this->totalServers = k * k * k / 4;
    this->numVertices = this->numCoreSwitch + this->numPods * this->numSwitchPerPod + this->totalServers;
    // create adjList and build Addresses
    this->addCoreToAgg();
    this->addAggToEdge();
    this->addEdgeToServer();
//    this->buildAddresses();

}

void FatTree::addCoreToAgg() {
    for (int core = 0; core < this->numCoreSwitch; ++core) {
        for (int pod = 0; pod < this->numPods; ++pod) {
            int offset = (int)(core / (int)(this->numSwitchPerPod / 2));
            int agg = this->numCoreSwitch + (int) (this->numSwitchPerPod / 2) * pod + offset;
            this->addEdge(core, agg);
        }
    }
}

void FatTree::addAggToEdge() {
    for (int pod = 0; pod < this->numPods; ++pod) {
        for (int numAgg = 0; numAgg < (int)(this->numSwitchPerPod / 2); ++numAgg) {
            int agg = this->numCoreSwitch + (int)(this->numSwitchPerPod / 2) * pod + numAgg;
            for (int numEdge = 0; numEdge < (int)(this->numSwitchPerPod / 2); ++numEdge) {
                int edge = this->numCoreSwitch + (int)(this->numSwitchPerPod / 2) * this->numPods + (int)(this->numSwitchPerPod / 2) * pod + numEdge;
                this->addEdge(agg, edge);
            }
        }
    }
}

void FatTree::addEdgeToServer() {
    for (int pod = 0; pod < this->numPods; ++pod) {
        for (int numEdge = 0; numEdge < (int)(this->numSwitchPerPod / 2); ++numEdge) {
            int edge = this->numCoreSwitch + (int)(this->numSwitchPerPod / 2) * this->numPods + (int)(this->numSwitchPerPod / 2) * pod + numEdge;
            for (int numServer = 0; numServer < (int)(this->paramK / 2); ++numServer) {
                int server = this->numCoreSwitch + this->numSwitchPerPod * this->numPods + (int)(this->numSwitchPerPod / 2) * numEdge + (int)(this->paramK / 2) * (int)(this->paramK / 2) * pod + numServer;
                this->addEdge(edge, server);
            }
        }
    }
}

int FatTree::getK() {
    return this->paramK;
}

int FatTree::getNumCoreSwitch() {
    return this->numCoreSwitch;
}

int FatTree::getNumPods() {
    return this->numPods;
}

int FatTree::getNumSwitchPerPod() {
    return this->numSwitchPerPod;
}

int FatTree::getNumServers() {
    return this->totalServers;
}


FatTree::FatTree() = default;
