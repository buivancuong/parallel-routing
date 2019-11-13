//
// Created by cuongbv on 14/10/2019.
//

#ifndef PARALLEL_ROUTING_EDGE_H
#define PARALLEL_ROUTING_EDGE_H


#include "Vertice.h"

class Edge {

protected:
    double weight{};
    bool directed{};
    std::pair<int, int> points;     // <source, destination>

public:
    // Constructor & Destructor
    explicit Edge(std::pair<int, int> verticesID);
    virtual ~Edge();

    // Getter & Setter
    double getWeight();
    bool getDirected();
    void setWeight(double newWeight);
    void setDirected(bool isDirected);



};


#endif //PARALLEL_ROUTING_EDGE_H
