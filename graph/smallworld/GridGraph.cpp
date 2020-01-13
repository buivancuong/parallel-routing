//
// Created by cuongbv on 26/11/2019.
//

#include <ctime>
#include <cmath>

#include "GridGraph.h"

#include <utility>
#include <algorithm>
#include <iostream>


GridGraph::GridGraph(int numRow, int numCol) {
    this->numRow = numRow;
    this->numCol = numCol;
    this->numVertices = numRow * numCol;
//    this->adjList = std::map<int, std::map<int, float> > ();
}

int GridGraph::getVertexID(int row, int col) {
    return row * this->numCol + col;
}


//
//void GridGraph::addTorusEdge(int currentVertex) {
//    this->addEdge(currentVertex, (currentVertex + this->numCol) % this->numVertices,
//                  (float) this->getGridHop(currentVertex, (currentVertex + this->numCol) % this->numCol));
//    this->addEdge(currentVertex, (currentVertex + 1) % this->numCol,
//                  (float) this->getGridHop(currentVertex, (currentVertex + 1) % this->numCol));
//}

int GridGraph::getGridHop(int sourceID, int destID) {
    int sourceRow = (int)(sourceID / this->numCol);
    int sourceCol = sourceID % this->numCol;
    int destRow = (int)(destID / this->numCol);
    int destCol = destID % this->numCol;
    return abs(sourceRow - destRow) + abs(sourceCol - destCol);
}

int GridGraph::getNumRow() {
    return this->numRow;
}

int GridGraph::getNumCol() {
    return this->numCol;
}

void GridGraph::generateFixedDegree(const std::vector<float> &alphas) {
    // for each value of alpha
    for (float alpha : alphas) {
        std::cout << "alpha = " << alpha << std::endl;
        // create temporary list of all Nodes
        std::vector<int> vertexList;
        for (const auto &item : this->adjList) {
            vertexList.push_back(item.first);
        }
        // Per loop, get random 1 Node, create randomlink with alpha param for that Node
        while (!vertexList.empty()) {
            std::cout << "leaf size " << vertexList.size() << std::endl;
            if (vertexList.size() == 2 and this->getGridHop(vertexList[0], vertexList[1]) != 1) {
                this->addEdge(vertexList[0], vertexList[1], this->getGridHop(vertexList[0], vertexList[1]));
                break;
            }
            // random select 1 Node ID
            std::srand((int) time(0));
            int randomIndex = std::rand() % (vertexList.size() - 1);
            int randomSource = vertexList[randomIndex];
            int randomDest = 0;
            // candidate list to create random link
            std::set<int> candidates;
            for (auto &candidate : vertexList) {
                // if the connect is existed, ignore
                if (!this->adjList[randomSource][candidate] and (candidate != randomSource)) {
                    candidates.insert(candidate);
                    this->adjList[randomSource].erase(candidate);
                }
            }
            // if have no candidate, break
            if (candidates.empty()) break;
            // delta value for per Node
            float delta = 0;
            // Count delta for per Node
            for (int candidate : candidates) {
                float smallworldRation = 1 / pow(this->getGridHop(randomSource, candidate), alpha);
                delta += smallworldRation;
            }
            // create candidate list with correspond connection probability
            std::map<int, float> candidateProb;
            for (int candidate : candidates) {
                candidateProb.insert(std::pair<int, float>(candidate, 1 / delta *
                                                                      this->getProbConnection(randomSource, candidate,
                                                                                              delta, alpha)));
            }
            // reconstruct candidate list with Cumulative Probability
            std::map<int, float> candidateCumulativeProb;
            float cumulativeProb = 0;
            for (std::pair<int, float> candidate : candidateProb) {
                cumulativeProb += candidate.second;
                candidateCumulativeProb.insert(std::pair<int, float>(candidate.first, cumulativeProb));
            }
            // select destID by pickup randomly value
            float randomValue = static_cast <float> (std::rand()) / (static_cast <float> (RAND_MAX/cumulativeProb));
            for (std::pair<int, float> candidate : candidateCumulativeProb) {
                if (candidate.second > randomValue) {
                    randomDest = candidate.first;
                    break;
                }
            }
            if (!randomDest) continue;
            this->addEdge(randomSource, randomDest, (float)this->getGridHop(randomSource, randomDest));
            std::cout << "add random link " << randomSource << " to " << randomDest << " weight " << (float)this->getGridHop(randomSource, randomDest) << std::endl;
            vertexList.erase(std::remove(vertexList.begin(), vertexList.end(), randomSource), vertexList.end());
            vertexList.erase(std::remove(vertexList.begin(), vertexList.end(), randomDest), vertexList.end());
        }
    }
}

float GridGraph::getProbConnection(int sourceID, int destID, float delta, float alpha) {
    float prob = 1 / (delta * pow(this->getGridHop(sourceID, destID), alpha));
    return prob;
}


GridGraph::~GridGraph() = default;
