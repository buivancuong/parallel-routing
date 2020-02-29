//
// Created by cuongbv on 18/02/2020.
//

#ifndef PARALLEL_ROUTING_FATTREENODE_H
#define PARALLEL_ROUTING_FATTREENODE_H


#include <vector>

class FatTreeNode {
private:
    int nodeID{};
    int layer{};      // 0 is Root (Core), 1 is Agg, 2 is Edge, 3 is Server
    int address[4]{};     // IP address of node
    std::vector<std::pair<std::vector<int>, int> > routingTable;
    

public:
    FatTreeNode();
    explicit FatTreeNode(int nodeID);
    FatTreeNode(int nodeID, int numCoreSwitch, int numPods, int numSwitchPerPod);
    ~FatTreeNode();

    int getNodeID();
    int getLayer();
    int* getAddress();

    void buildTable(int numCoreSwitch, int numPods, int numSwitchPerPod);
};


#endif //PARALLEL_ROUTING_FATTREENODE_H
