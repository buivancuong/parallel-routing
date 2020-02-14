//
// Created by cuongbv on 23/01/2020.
//

#ifndef PARALLEL_ROUTING_FATTREEUTILS_H
#define PARALLEL_ROUTING_FATTREEUTILS_H


class FatTreeUtils {

public:
    FatTreeUtils();
    ~FatTreeUtils();

    static bool isCoreSwitch(int nodeID, int numCoreSwitch);
    static bool isAggSwitch(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);
    static bool isEdgeSwitch(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);
    static bool isServer(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);
    static bool isInPod(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);

    static int getPod(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);

};


#endif //PARALLEL_ROUTING_FATTREEUTILS_H
