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
    if (k > 256) {
        std::cout << "k must be less than  256" << std::endl;
        return;
    }
    // constructs parameters of Fat tree
    this->paramK = k;
    this->numCoreSwitchs = k * k / 4;
    this->numPods = k;
    this->numSwithPerPod = k;
    this->numServers = k * k * k / 4;
    this->numVertices = this->numCoreSwitchs + this->numSwithPerPod + this->numServers;
    // create adjList and build Addresses
    this->addCoreToAgg();
    this->addAggToEdge();
    this->addEdgeToServer();
    this->buildAddresses();

}

void FatTree::addCoreToAgg() {
    for (int core = 0; core < this->numCoreSwitchs; ++core) {
        for (int pod = 0; pod < this->numPods; ++pod) {
            int offset = (int)(core / (int)(this->numSwithPerPod / 2));
            int agg = this->numCoreSwitchs + (int) (this->numSwithPerPod / 2) * pod + offset;
            this->addEdge(core, agg);
        }
    }
}

void FatTree::addAggToEdge() {
    for (int pod = 0; pod < this->numPods; ++pod) {
        for (int numAgg = 0; numAgg < (int)(this->numSwithPerPod / 2); ++numAgg) {
            int agg = this->numCoreSwitchs + (int)(this->numSwithPerPod / 2) * pod + numAgg;
            for (int numEdge = 0; numEdge < (int)(this->numSwithPerPod / 2); ++numEdge) {
                int egg =
            }
        }
    }
}

int FatTree::getK() {
    return this->paramK;
}

int FatTree::getNumCoreSwitch() {
    return this->numCoreSwitchs;
}

int FatTree::getNumPods() {
    return this->numPods;
}

int FatTree::getNumSwitchPerPod() {
    return this->numSwithPerPod;
}

int FatTree::getNumServers() {
    return this->numServers;
}

FatTree::FatTree() = default;
