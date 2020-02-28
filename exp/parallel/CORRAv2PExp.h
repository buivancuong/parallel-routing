//
// Created by cuongbv on 21/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRAV2PEXP_H
#define PARALLEL_ROUTING_CORRAV2PEXP_H


#include <map>
#include "../../node/Node.h"
#include "../../graph/Graph.h"

class CORRAv2PExp {
private:
    std::map<int, Node*> corra2NodeList;

public:
    void createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    void addNearFarNeighbors(int startNodeID, int endNodeID, Graph *graph);

    void prepareLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    void createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize,int yTopoSize);

    void createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize);

    void findBR1(int startNodeID, int endNodeID);

    void findBRn(int startNodeID, int endNodeID, int n = 2);

    void broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize);

    void handleMissingBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    void updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    CORRAv2PExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float>& alphas, int numSubThread);
    ~CORRAv2PExp();
};


#endif //PARALLEL_ROUTING_CORRAV2PEXP_H
