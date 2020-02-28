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


//std::map<int, std::pair<float, int> > Graph::Dijkstra(int sourceID) {
//    auto start = std::chrono::system_clock::now();
//    std::cout << "Dijkstra " << sourceID << std::endl;
//    // Initialize start status
//    std::map<int, std::pair<float, int> > trace;
//    std::map<int, bool> visited;
//    for (int i = 0; i < this->numVertices; ++i) {
//        visited.insert(std::pair<int, bool>(i, false));
//        if (i == sourceID) {
//            std::pair<float, int> dist_prev (0, sourceID);
//            trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
//        } else {
//            std::pair<float, int> dist_prev (INFTY, -1);
//            trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
//        }
//    }
//
//    for (int i = 0; i < this->numVertices; ++i) {
//        int currentVertexID = -1;
//        // Check if all candidate is INFTY and find the min vertex
//        auto start1 = std::chrono::system_clock::now();
//        for (std::pair<const int, bool> iterator : visited) {
//            if (iterator.second) continue;
//            if (currentVertexID == -1) {
//                currentVertexID = iterator.first;
//                continue;
//            }
//            if (trace[iterator.first].first < trace[currentVertexID].first) {
//                currentVertexID = iterator.first;
//            }
//        }
//        auto start2 = std::chrono::system_clock::now();
//        if (trace[currentVertexID].first == INFTY) break;
//        // Perform a Dijkstra iteration
//        visited[currentVertexID] = true;
//        for (std::pair<int, float> iterator : this->adjList[currentVertexID]) {
//            int destID = iterator.first;
//            float distance = iterator.second;
//            if (trace[currentVertexID].first + distance < trace[destID].first) {
//                trace[destID].first = trace[currentVertexID].first + distance;
//                trace[destID].second = currentVertexID;
//            }
//        }
//        auto start3 = std::chrono::system_clock::now();
//        std::chrono::duration<double> elapsed_seconds1 = start2 - start1;
//        std::chrono::duration<double> elapsed_seconds2 = start3 - start2;
//        std::cout << "Ty so " << (elapsed_seconds1.count() / elapsed_seconds2.count()) << std::endl;
//        visited.erase(currentVertexID);
//    }

//    auto end = std::chrono::system_clock::now();
//    std::chrono::duration<double> elapsed_seconds = end-start;
//    std::cout << "Time Dijkstra " << elapsed_seconds.count() << std::endl;
//    return trace;
//}

Graph::~Graph() = default;
