//
// Created by cuongbv on 26/11/2019.
//

#include <vector>
#include "../graph/Graph.h"

#ifndef PARALLEL_ROUTING_ROUTINGALGORITHM_H
#define PARALLEL_ROUTING_ROUTINGALGORITHM_H

#endif //PARALLEL_ROUTING_ROUTINGALGORITHM_H

class RoutingAlgorithm {
public:
    virtual void preparingRouting(Graph graph, int sourceID) = 0;
    virtual std::vector<int> routing(int sourceID, int destID) = 0;
};