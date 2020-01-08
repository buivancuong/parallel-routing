//
// Created by cuongbv on 26/11/2019.
//

#ifndef PARALLEL_ROUTING_DIJKSTRAALGORITHM_H
#define PARALLEL_ROUTING_DIJKSTRAALGORITHM_H

#include "RoutingAlgorithm.h"

class DijkstraAlgorithm : public RoutingAlgorithm {
private:
    std::map<int, std::pair<float, int> > trace;

public:
    DijkstraAlgorithm();
    ~DijkstraAlgorithm();

    void preparingRouting(Graph graph, int sourceID) override;

    std::vector<int> routing(int sourceID, int destID) override;
};


#endif //PARALLEL_ROUTING_DIJKSTRAALGORITHM_H
