//
// Created by cuongbv on 26/11/2019.
//

#define INFTY 999999999

#include "DijkstraAlgorithm.h"

void DijkstraAlgorithm::preparingRouting(Graph graph, int sourceID) {
    std::map<int, bool> visited;
    for (int i = 0; i < graph.getNumVertices(); ++i) {
        visited.insert(std::pair<int, bool>(i, false));
        if (i == sourceID) {
            std::pair<float, int> dist_prev (0, sourceID);
            trace.insert((std::pair<int, std::pair<float, int> >(i, dist_prev)));
        } else {
            std::pair<float, int> dist_prev (INFTY, -1);
            trace.insert(std::pair<int, std::pair<float, int> >(i, dist_prev));
        }
    }

    for (int i = 0; i < graph.getNumVertices(); ++i) {
        int currentVertexID = -1;
        for (std::pair<const int, bool> iterator : visited) {
            if (!iterator.second) {
                if ((currentVertexID == -1) or (trace[iterator.first].first < trace[currentVertexID].first)) {
                    currentVertexID = iterator.first;
                }
            }
        }
        if (trace[currentVertexID].first == INFTY) break;
        // Perform the Dijkstra iteration
        visited[currentVertexID] = true;
        for (std::pair<int, float> iterator : graph.getAdjList()[currentVertexID]) {
            int destID = iterator.first;
            float distance = iterator.second;
            if (trace[currentVertexID].first + distance < trace[destID].first) {
                trace[destID].first = trace[currentVertexID].first + distance;
                trace[destID].second = currentVertexID;
            }
        }
    }
}

std::vector<int> DijkstraAlgorithm::routing(int sourceID, int destID) {
    std::vector<int> path;
    path.push_back(destID);
    while (true) {
        int prevID = this->trace[path.back()].second;
        if (prevID == path.back()) break;
        else path.push_back(prevID);
    }
    return path;
}

DijkstraAlgorithm::~DijkstraAlgorithm() = default;

DijkstraAlgorithm::DijkstraAlgorithm() = default;
