//
// Created by cuongbv on 22/12/2019.
//

#ifndef PARALLEL_ROUTING_EDGE_H
#define PARALLEL_ROUTING_EDGE_H

#include "Node.h"

class Edge {
protected:
    double weight{};
    std::pair<int, int> points;     // <sourceID, destinationID>

public:
    // Constructor & Destructor
    explicit Edge(std::pair<int, int> verticesID, int xTopoSize, int yTopoSize);
    explicit Edge(Edge *edge);
    ~Edge();

    // Getter & Setter
    double getWeight();
    std::pair<int, int> getPoints();
    void setWeight(double newWeight);

    // Compare Edge(source-dest) = Edge(dest-source)
    static bool isExistEdges(const Edge& firstEdge, const Edge& secondEdge);
};


#endif //PARALLEL_ROUTING_EDGE_H
