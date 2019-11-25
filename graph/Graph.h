//
// Created by cuongbv on 25/11/2019.
//

#ifndef PARALLEL_ROUTING_GRAPH_H
#define PARALLEL_ROUTING_GRAPH_H

#include <map>
#include <set>

class Graph {
protected:
    int numVertices;
    int numEdges{};
    std::map<int, std::map<int, float> > adjList;

public:
    // Constructor & Destructor
    Graph();
    explicit Graph(int totalVertice);
    ~Graph();

    // Getter
    int getNumVertices();
    int getNumEdges();
    std::map<int, std::map<int, float> > getAdjList();

    // Add 1 vertex
    void addVertex(int vertexID);
    // Add 1 edge with default weight = 1
    void addEdge(int sourceID, int destID, float weight = 1);

    // Default Dijkstra shortest path routing from specifix sourceID
    std::map<int, std::pair<float, int> > Dijkstra(int sourceID);       // <destID, <distance, previousID> >

};


#endif //PARALLEL_ROUTING_GRAPH_H
