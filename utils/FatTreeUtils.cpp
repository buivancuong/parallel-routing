//
// Created by cuongbv on 23/01/2020.
//

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
        return (int)((nodeID - numCoreSwitch - (int)(numSwitchPerPod / 2)) / (int)(numSwitchPerPod / 2));
    }
    return -1;
}
