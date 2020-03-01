//
// Created by cuongbv on 28/11/2019.
//

#ifndef PARALLEL_ROUTING_SMALLWORLD2DGRID_H
#define PARALLEL_ROUTING_SMALLWORLD2DGRID_H


#include <vector>
#include "GridGraph.h"

class SmallWorld2DGrid : public GridGraph {
private:
    std::vector<float> alphas;

    void addGridEdge(int currentVertex);

public:
    SmallWorld2DGrid(int numRow, int numCol, const std::vector<float>& alphas);
};


#endif //PARALLEL_ROUTING_SMALLWORLD2DGRID_H
