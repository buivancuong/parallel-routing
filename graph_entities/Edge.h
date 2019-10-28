//
// Created by cuongbv on 14/10/2019.
//

#ifndef PARALLEL_ROUTING_EDGE_H
#define PARALLEL_ROUTING_EDGE_H


#include "Vertice.h"

class Edge {

protected:
    float weight{};
    bool directed{};
    Vertice vertices[2];

public:
    // Constructor & Destructor
    explicit Edge(int *verticesId);
    virtual ~Edge();

    // Getter & Setter
    float getWeight();
    bool getDirected();
    void setWeight(int newWeight);



};


#endif //PARALLEL_ROUTING_EDGE_H
