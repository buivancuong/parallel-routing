//
// Created by cuongbv on 18/02/2020.
//

#include <iostream>
#include "FatTreeNode.h"
#include "../utils/FatTreeUtils.h"

FatTreeNode::FatTreeNode() = default;

FatTreeNode::FatTreeNode(int nodeID) {
    this->nodeID = nodeID;
    this->address[0] = 10;
}

FatTreeNode::FatTreeNode(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    std::cout << "Node " << nodeID << std::endl;
    this->nodeID = nodeID;
    this->address[0] = 10;
    int* tempAddress = FatTreeUtils::nodeIDToAddress(nodeID, numCoreSwitch, numPods, numSwitchPerPod);
    this->address[1] = tempAddress[1];
    this->address[2] = tempAddress[2];
    this->address[3] = tempAddress[3];

    if (this->address[1] == numPods) {
        this->layer = 0;
        return;
    }
    if (this->address[3] != 1) {
        this->layer = 3;
        return;
    }
    if (this->address[2] >= (int)(numPods / 2)) {
        this->layer = 1;
        return;
    }
    else this->layer = 2;
}

int *FatTreeNode::getAddress() {
    return this->address;
}

int FatTreeNode::getLayer() {
    return this->layer;
}

int FatTreeNode::getNodeID() {
    return this->nodeID;
}

void FatTreeNode::buildTable(int numCoreSwitch, int numPods, int numSwitchPerPod) {
    // if this node is Server, have no routing table
    if (this->layer == 3) return;
    // if this node is root
    if (this->layer == 0) {
        for (int pod = 0; pod < numPods; ++pod) {
            int aggOffset = this->address[2];
            int agg = numCoreSwitch + pod * (int)(numPods / 2) + aggOffset - 1;
            int prefix[4];
            prefix[0] = 10;
            prefix[1] = pod;
            prefix[2] = -1;
            prefix[3] = -1;
            this->routingTable.emplace_back(std::pair<int*, int>(prefix, agg));
        }
        return;
    }
    // if this node is Edge
    if (this->address[2] < (int)(numPods / 2)) {
        // Edge to Server
        for (int serverID = 2; serverID < (int)(numPods / 2) + 2; ++ serverID) {
            int server = numCoreSwitch + numPods * numSwitchPerPod + (int)(numPods * numPods / 4) * this->address[1] + (int)(numPods / 2) * this->address[2] + serverID - 2;
            int suffix[4];
            suffix[0] = 10;
            suffix[1] = this->address[2];
            suffix[2] = this->address[3];
            suffix[3] = serverID;
            this->routingTable.emplace_back(std::pair<int*, int>(suffix, server));
        }
        // Edge to Agg
        for (int aggID = (int)(numPods / 2); aggID < numPods; ++ aggID) {
            int agg = numCoreSwitch + this->address[1] * (int)(numPods / 2) + this->address[2] - (int)(numPods / 2);
            int suffix[4];
            suffix[0] = 10;
            suffix[1] = -1;
            suffix[2] = -1;
            suffix[3] = aggID - (int)(numPods / 2) + 2;
            this->routingTable.emplace_back(std::pair<int*, int>(suffix, agg));
        }
        return;
    } else {        // if this node is Agg
        // Agg to Edge
        for (int edgeID = 0; edgeID < (int)(numPods / 2); ++edgeID) {
            int edge = numCoreSwitch + numPods * (int)(numSwitchPerPod / 2) + this->address[1] * (int)(numPods / 2) + edgeID;
            int prefix[4];
            prefix[0] = 10;
            prefix[1] = this->address[1];
            prefix[2] = edgeID;
            prefix[3] = -1;
            this->routingTable.emplace_back(std::pair<int*, int>(prefix, edge));
        }
        // Agg to Core
        for (int serverSuffix = 2; serverSuffix < (int)(numPods / 2) + 2; ++serverSuffix) {
            int core = (this->address[2] - (int)(numPods / 2)) * (int)(numPods / 2) + serverSuffix - 2;
            int suffix[4];
            suffix[0] = -1;
            suffix[1] = -1;
            suffix[2] = -1;
            suffix[3] = serverSuffix;
            this->routingTable.emplace_back(std::pair<int*, int>(suffix, core));
        }
        return;
    }
}

FatTreeNode::~FatTreeNode() = default;
