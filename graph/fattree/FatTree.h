//
// Created by cuongbv on 23/01/2020.
//

#ifndef PARALLEL_ROUTING_FATTREE_H
#define PARALLEL_ROUTING_FATTREE_H


#include "../Graph.h"

class FatTree : public Graph{
private:
    int paramK{};     // number of ports per switch
    int numCoreSwitch{};     // number of Core Switch (root core)
    int numPods{};        // number of Pod (consist Agg Switch and Edge Switch)
    int numSwitchPerPod{};     // number of switch (include both of Agg and Edge) on 1 pod
    int totalServers{};     // total of servers below Edge layer
    // methods to add edges on Fat tree
    void addCoreToAgg();
    void addAggToEdge();
    void addEdgeToServer();
    // method to build IP addresses for Fat tree
//    void buildAddresses();

public:
    explicit FatTree(int k);
    FatTree();
    ~FatTree();
    // getter & setter
    int getK();
    int getNumCoreSwitch();
    int getNumPods();
    int getNumSwitchPerPod();
    int getNumServers();

};


#endif //PARALLEL_ROUTING_FATTREE_H
