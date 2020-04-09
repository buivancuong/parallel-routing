//
// Created by cuongbv on 25/11/2019.
//

#define INFTY 999999999

#include "Graph.h"
#include <queue>
#include <iostream>
#include <chrono>
#include <list>
#include <algorithm>

Graph::Graph() {
    this->numVertices = 0;
    this->numEdges = 0;
}

Graph::Graph(int totalVertice) {
    this->numVertices = totalVertice;
}

int Graph::getNumVertices() {
    return this->numVertices;
}

int Graph::getNumEdges() {
    return this->numEdges;
}

std::map<int, std::map<int, float> > Graph::getAdjList() {
    return this->adjList;
}

void Graph::addVertex(int vertexID) {
    this->numVertices++;
    std::map<int, float> neighborList;
    this->adjList.insert(std::pair<int, std::map<int, float > >(vertexID, neighborList));
}

void Graph::addEdge(int sourceID, int destID, float weight) {
    if (weight == 0) return;
    if (sourceID == destID) return;
    this->numEdges++;
    this->adjList[sourceID].insert(std::pair<int, float>(destID, weight));
    this->adjList[destID].insert(std::pair<int, float>(sourceID, weight));
}

std::map<int, std::pair<float, int> > Graph::Dijkstra(int sourceID, float nodeLatency) {
    float distance[this->numVertices];
    int previous[this->numVertices];
    std::list<int> nodeList;

    for (int i = 0; i < this->numVertices; ++i) {
        distance[i] = INFTY;
        previous[i] = -1;
        nodeList.push_back(i);
    }
    distance[sourceID] = 0;
    previous[sourceID] = sourceID;

    while (!nodeList.empty()) {
        int currentVertex = nodeList.front();
        for (int i : nodeList) {
            if (distance[i] < distance[currentVertex]) {
                currentVertex = i;
            }
        }

        nodeList.remove(currentVertex);
        std::map<int, float> temp = this->adjList[currentVertex];
        for (std::pair<int, float> neighbor : temp) {
            if (distance[currentVertex] + neighbor.second + nodeLatency < distance[neighbor.first]) {
                distance[neighbor.first] = distance[currentVertex] + neighbor.second + nodeLatency;
                previous[neighbor.first] = currentVertex;
            }
        }
    }

    std::map<int, std::pair<float, int> > trace;
    for (int i = 0; i < this->numVertices; ++i) {
        std::pair<float, int> dist_prev(distance[i], previous[i]);
        trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
    }
    return trace;
}

Graph::~Graph() = default;
