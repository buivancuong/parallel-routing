//
// Created by cuongbv on 21/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRAV1PEXP_H
#define PARALLEL_ROUTING_CORRAV1PEXP_H


#include <vector>
#include <map>
#include "../../node/CORRANode.h"
#include "../../utils/CORRAUtils.h"

class CORRAv1PExp {
private:
    std::map<int, CORRANode*> nodeList;

public:

    void createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

    void createNearFarNeighbors(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize,int yTopoSize);

    void createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize,int yTopoSize);

    void createLocalRouting(int startNodeID, int endNodeID, int xTopoSize, int yTopoSize);


    CORRAv1PExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float>& alphas, int numSubThread);

};


#endif //PARALLEL_ROUTING_CORRAV1PEXP_H
