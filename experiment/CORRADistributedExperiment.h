//
// Created by cuongbv on 05/01/2020.
//

#ifndef PARALLEL_ROUTING_CORRADISTRIBUTEDEXPERIMENT_H
#define PARALLEL_ROUTING_CORRADISTRIBUTEDEXPERIMENT_H


#include "../graph/smallworld/SmallWorld2DGrid.h"

class CORRADistributedExperiment {
private:
    int xTopoSize;
    int yTopoSize;
    int xBlockSize;
    int yBlockSize;
    int deltaNeighbor;
    std::vector<double> alphas;

public:
    CORRADistributedExperiment();
    ~CORRADistributedExperiment();

};


#endif //PARALLEL_ROUTING_CORRADISTRIBUTEDEXPERIMENT_H
