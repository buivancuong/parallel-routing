//
// Created by cuongbv on 19/12/2019.
//

#ifndef PARALLEL_ROUTING_CORRADISTRIBUTED_H
#define PARALLEL_ROUTING_CORRADISTRIBUTED_H


#include "RoutingAlgorithm.h"

class CORRADistributed : public RoutingAlgorithm {

private:
    int deltaNeighbor{};
    int xBlockSize{};
    int yBlockSize{};




public:
    CORRADistributed();
    ~CORRADistributed();

    void createLocality(Graph graph, int sourceID);
    void findAllBridges(Graph graph, int sourceID);
    void updateBridgeTable(Graph graph, int sourceID);
    void handleMissingBridge(Graph graph, int sourceID);

    void preparingRouting(Graph graph, int sourceID) override;


};


#endif //PARALLEL_ROUTING_CORRADISTRIBUTED_H
