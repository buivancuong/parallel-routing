//
// Created by cuongbv on 22/02/2020.
//

#ifndef PARALLEL_ROUTING_TZPEXP_H
#define PARALLEL_ROUTING_TZPEXP_H


#include <map>
#include <vector>
#include "../../node/TZNode.h"

class TZPExp {
private:
    std::map<int, TZNode*> tzNodeList;

public:

    void createTZNodeList(int startNodeID, int endNodeID);

    void createTraceMap(int startNodeID, int endNodeID, Graph *globalGraph);

    void extractClosetLandmark(int startNodeID, int endNodeID, const std::map<int, TZNode*>& landmarks);

    void extractCluster(int startNodeID, int endNodeID);

    TZPExp(int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float>& alphas, int numSubThread);

};


#endif //PARALLEL_ROUTING_TZPEXP_H
