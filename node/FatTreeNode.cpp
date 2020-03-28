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
    // std::cout << "Node " << nodeID << std::endl;
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
    // if this node is root: onlt Root downto Agg
    if (this->layer == 0) {
        for (int pod = 0; pod < numPods; ++pod) {
            int aggOffset = this->address[2];
            int agg = numCoreSwitch + pod * (int)(numPods / 2) + aggOffset - 1;
            int prefixArray[4];
            prefixArray[0] = 10;
            prefixArray[1] = pod;
            prefixArray[2] = -1;
            prefixArray[3] = -1;
            std::vector<int> prefix;
            for (int i : prefixArray) {
                prefix.push_back(i);
            }
            this->routingTable.emplace_back(std::pair<std::vector<int>, int>(prefix, agg));
        }
        return;
    }
    // if this node is Edge
    if (this->address[2] < (int)(numPods / 2)) {
        // Edge downto Server
        for (int serverID = 2; serverID < (int)(numPods / 2) + 2; ++serverID) {
            int server = numCoreSwitch + numPods * numSwitchPerPod + (int)(numPods * numPods / 4) * this->address[1] + (int)(numPods / 2) * this->address[2] + serverID - 2;
            int suffixArray[4];
            suffixArray[0] = 10;
            suffixArray[1] = this->address[1];
            suffixArray[2] = this->address[2];
            suffixArray[3] = serverID;
            std::vector<int> suffix;
            for (int i : suffixArray) {
                suffix.push_back(i);
            }
            this->routingTable.emplace_back(std::pair<std::vector<int>, int>(suffix, server));
        }
        // Edge upto Agg
        for (int aggID = (int)(numPods / 2); aggID < numPods; ++aggID) {
            int agg = numCoreSwitch + this->address[1] * (int)(numPods / 2) + this->address[2] - (int)(numPods / 2);
            int suffixArray[4];
            suffixArray[0] = 10;        // X.X.X.2/8
            suffixArray[1] = -1;
            suffixArray[2] = -1;
            suffixArray[3] = aggID - (int)(numPods / 2) + 2;
            std::vector<int> suffix;
            for (int i : suffixArray) {
                suffix.push_back(i);
            }
            this->routingTable.emplace_back(std::pair<std::vector<int>, int>(suffix, agg));
        }
        return;
    } else {        // if this node is Agg
        // Agg downto Edge
        for (int edgeID = 0; edgeID < (int)(numPods / 2); ++edgeID) {
            int edge = numCoreSwitch + numPods * (int)(numSwitchPerPod / 2) + this->address[1] * (int)(numPods / 2) + edgeID;
            int prefixArray[4];
            prefixArray[0] = 10;        // 10.2.0.X/24
            prefixArray[1] = this->address[1];
            prefixArray[2] = edgeID;
            prefixArray[3] = -1;
            std::vector<int> prefix;
            for (int i : prefixArray) {
                prefix.push_back(i);
            }
            this->routingTable.emplace_back(std::pair<std::vector<int>, int>(prefix, edge));
        }
        // Agg upto Core
        for (int serverSuffix = 2; serverSuffix < (int)(numPods / 2) + 2; ++serverSuffix) {
            int core = (this->address[2] - (int)(numPods / 2)) * (int)(numPods / 2) + serverSuffix - 2;
            int suffixArray[4];
            suffixArray[0] = 10;        // X.X.X.2/8
            suffixArray[1] = -1;
            suffixArray[2] = -1;
            suffixArray[3] = serverSuffix;
            std::vector<int> suffix;
            for (int i : suffixArray) {
                suffix.push_back(i);
            }
            this->routingTable.emplace_back(std::pair<std::vector<int>, int>(suffix, core));
        }
        return;
    }
}

std::vector<std::pair<std::vector<int>, int> > FatTreeNode::getRoutingTable() {
    return this->routingTable;
}

void FatTreeNode::updateRoutingTable(const std::vector<int>& destMaskIPAddress, int nextNodeID) {
    this->routingTable.emplace_back(destMaskIPAddress, nextNodeID);
}

FatTreeNode::~FatTreeNode() = default;
