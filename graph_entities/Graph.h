//
// Created by cuongbv on 13/11/2019.
//

#ifndef PARALLEL_ROUTING_GRAPH_H
#define PARALLEL_ROUTING_GRAPH_H


#include <map>
#include <vector>
#include <set>
#include "Edge.h"
#include "Vertice.h"

class Graph {

protected:
    int totalNode{};
    std::map<int, Vertice> verticesList;        // <verticeID, vertice>
    std::map<int, std::map<int, Edge*> > adjList;     // <nodeID, neighbors<neighborID, Edge(nodeID <--> neighborID>

public:
    // Constructor & Destructor
    Graph();
    explicit Graph(int totalNode);
    ~Graph();

    // Getter
    int getTotalNode();
    std::map<int, std::map<int, Edge*> > getAdjList();
    std::map<int, Edge*> getNeighbors(int verticeID);

    void addVertice(Vertice newVertice);
    void addEdge(int source, int dest);


    // Shortest path routing

};


#endif //PARALLEL_ROUTING_GRAPH_H
