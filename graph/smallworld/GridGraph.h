//
// Created by cuongbv on 26/11/2019.
//

#ifndef PARALLEL_ROUTING_GRIDGRAPH_H
#define PARALLEL_ROUTING_GRIDGRAPH_H


#include <vector>
#include "../Graph.h"

class GridGraph : public Graph {

protected:
    int numRow;
    int numCol;
    void generateFixedDegree(const std::vector<float>& alphas);

public:
    // Constructor & Destructor
    GridGraph(int numRow, int numCol);
    ~GridGraph();

    // Getter
    int getNumRow();
    int getNumCol();

    int getVertexID(int row, int col);
    int getGridHop(int sourceID, int destID);
    float getProbConnection(int sourceID, int destID, float delta, float alpha);
};


#endif //PARALLEL_ROUTING_GRIDGRAPH_H
