//
// Created by cuongbv on 28/11/2019.
//

#include "SmallWorld2DGrid.h"

SmallWorld2DGrid::SmallWorld2DGrid(int numRow, int numCol, const std::vector<float>& alphas) : GridGraph(numRow, numCol) {
    this->alphas = alphas;
    for (int i = 0; i < this->getNumVertices(); ++ i) {
        this->addGridEdge(i);
    }
    this->alphas = alphas;
    this->generateFixedDegree(this->alphas);
    for (std::pair<int, std::map<int, float> > vertex : this->adjList) {
        std::map<int, float> temp = vertex.second;
        for (std::pair<int, float> neighbor : temp) {
            if (neighbor.first == vertex.first){
                this->adjList[vertex.first].erase(neighbor.first);
            }
        }
    }
}

void SmallWorld2DGrid::addGridEdge(int currentVertex) {
    int currentRow = (int)(currentVertex / this->numCol);
    int currentCol = currentVertex % this->numCol;
    if (currentRow < (this->numRow - 1)) this->addEdge(currentVertex, currentVertex + this->numCol, 1);
    if (currentCol < (this->numCol - 1)) this->addEdge(currentVertex, currentVertex + 1, 1);
}
