//
// Created by cuongbv on 25/11/2019.
//

#define INFTY 999999999

#include "Graph.h"
#include <queue>
#include <iostream>

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
    if (sourceID == 1 or destID == 1) {
        if (weight != 1) {
            std::cout << "add random link " << sourceID << " " << destID << " " << weight << std::endl;
        }
    }
    this->numEdges++;
    this->adjList[sourceID].insert(std::pair<int, float>(destID, weight));
    this->adjList[destID].insert(std::pair<int, float>(sourceID, weight));
}

std::map<int, std::pair<float, int> > Graph::Dijkstra(int sourceID) {
    // Initialize start status
    std::map<int, std::pair<float, int> > trace;
    std::map<int, bool> visited;
    for (int i = 0; i < this->numVertices; ++i) {
        visited.insert(std::pair<int, bool>(i, false));
        if (i == sourceID) {
            std::pair<float, int> dist_prev (0, sourceID);
            trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
        } else {
            std::pair<float, int> dist_prev (INFTY, -1);
            trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
        }
    }

    for (int i = 0; i < this->numVertices; ++i) {
        int currentVertexID = -1;
        // Check if all candidate is INFTY
        for (std::pair<const int, bool> iterator : visited) {
            if (!iterator.second) {
                if ((currentVertexID == -1 ) or (trace[iterator.first].first < trace[currentVertexID].first)) {
                    currentVertexID = iterator.first;
                }
            }
        }
        if (trace[currentVertexID].first == INFTY) break;
        // Perform a Dijkstra iteration
        visited[currentVertexID] = true;
        for (std::pair<int, float> iterator : this->adjList[currentVertexID]) {
            int destID = iterator.first;
            float distance = iterator.second;
            if (trace[currentVertexID].first + distance < trace[destID].first) {
                trace[destID].first = trace[currentVertexID].first + distance;
                trace[destID].second = currentVertexID;
            }
        }
    }
    return trace;
}

Graph::~Graph() = default;
