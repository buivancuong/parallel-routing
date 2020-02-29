//
// Created by cuongbv on 23/01/2020.
//

#include <iostream>
#include "FatTreeUtils.h"

FatTreeUtils::FatTreeUtils() = default;

FatTreeUtils::~FatTreeUtils() = default;

bool FatTreeUtils::isCoreSwitch(int nodeID, int numCoreSwitch) {
    return nodeID < numCoreSwitch;
}

bool FatTreeUtils::isAggSwitch(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    if (nodeID >= numCoreSwitch) {
        return nodeID < numCoreSwitch + numPods * (int) (numSwitchPerPod / 2);
    }
    return false;
}

bool FatTreeUtils::isEdgeSwitch(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    if (nodeID >= numCoreSwitch + numPods * (int)(numSwitchPerPod / 2)) {
        return nodeID < numCoreSwitch + numPods * numSwitchPerPod;
    }
    return false;
}

bool FatTreeUtils::isServer(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    return nodeID >= numCoreSwitch + numPods * numSwitchPerPod;
}

bool FatTreeUtils::isInPod(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    if (nodeID >= numCoreSwitch) {
        return nodeID < numCoreSwitch + numPods * numSwitchPerPod;
    }
    return false;
}

int FatTreeUtils::getPod(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    if (FatTreeUtils::isAggSwitch(nodeID, numCoreSwitch, numPods, numSwitchPerPod)) {
        return (int)((nodeID - numCoreSwitch) / (int)(numSwitchPerPod / 2));
    }
    if (FatTreeUtils::isEdgeSwitch(nodeID, numCoreSwitch, numPods, numSwitchPerPod)) {
        return (int)((nodeID - numCoreSwitch - numPods * (int)(numSwitchPerPod / 2)) / (int)(numSwitchPerPod / 2));
    }
    return -1;
}

int FatTreeUtils::addressToNodeID(int *address, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    // is Core switch
    if (address[1] == numPods) {
        return (int)((address[2] - 1) * numPods / 2 + address[3] - 1);
    }
    // is Server
    if (address[3] != 1) {
        return (int)(numCoreSwitch + numPods * numSwitchPerPod + address[1] * (int)(numPods * numPods / 4) + address[2] * (int)(numPods / 2) + address[3] - 3);
    }
    // is Agg
    if (address[2] >= (int)(numPods / 2)) {
        return (int)(numCoreSwitch + address[1] * (int)(numPods / 2) + (int)(address[2] - numPods / 2));
    } else {        // is Edge
        return (int)(numCoreSwitch + (address[1] + numPods) * (int)(numPods / 2) + address[2]);
    }
}

int* FatTreeUtils::nodeIDToAddress(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    int result[4];
    int *address = reinterpret_cast<int *>(&result);
    address[0] = 10;
    if (FatTreeUtils::isCoreSwitch(nodeID, numCoreSwitch)) {
        address[1] = numPods;     // numPods = k (k-port switch)
        address[2] = (int)(nodeID / (int)(numPods / 2)) + 1;
        address[3] = nodeID % (int)(numPods / 2) + 1;
        return address;
    }

    if (FatTreeUtils::isInPod(nodeID, numCoreSwitch, numPods, numSwitchPerPod)) {
        int podID = FatTreeUtils::getPod(nodeID, numCoreSwitch,numPods, numSwitchPerPod);
        address[1] = podID;       // pod
        address[3] = 1;       // gateway
        int column = ((nodeID - numPods) % (int)(numPods * numSwitchPerPod / 2)) % (int)(numPods / 2);
        std::cout << "column " << column << std::endl;
        if (FatTreeUtils::isAggSwitch(nodeID, numCoreSwitch, numPods, numSwitchPerPod)) {
            column += (int) (numPods / 2);
        }       // else is Edge switch
        address[2] = column;      // switch
        return address;
    }

    if (FatTreeUtils::isServer(nodeID, numCoreSwitch, numPods, numSwitchPerPod)) {
        // pod
        address[1] = (int)((nodeID - numCoreSwitch - numPods * numSwitchPerPod) / (int)(numPods * numPods / 4));
        // switch
        address[2] = (int)((nodeID - numCoreSwitch - numPods * numSwitchPerPod - address[1] * (int)(numPods * numPods / 4)) / (int)(numPods / 2));
        // ID
        address[3] = nodeID - numCoreSwitch - numPods * numSwitchPerPod - address[1] * (int)(numPods * numPods / 4) - address[2] * (int)(numPods / 2) + 2;
        return address;
    }
}
