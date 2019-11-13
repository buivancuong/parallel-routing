//
// Created by cuongbv on 14/10/2019.
//

#include "Edge.h"
#include "../utils/VerticeUtils.h"
#include "Vertice.h"


Edge::Edge(std::pair<int, int> verticesID) {
    this->points = verticesID;

    int firstHorizontal = VerticeUtils::getVerticeHorizontal(this->points.first, Vertice::xTopoSize);
    int firstVertical = VerticeUtils::getVerticeVertical(this->points.first, Vertice::yTopoSize);
    int secondHorizontal = VerticeUtils::getVerticeHorizontal(this->points.second, Vertice::xTopoSize);
    int secondVertical = VerticeUtils::getVerticeVertical(this->points.second, Vertice::yTopoSize);


}

double Edge::getWeight() {
    return this->weight;
}

bool Edge::getDirected() {
    return this->directed;
}

void Edge::setWeight(double newWeight) {
    this->weight = newWeight;
}

void Edge::setDirected(bool isDirected) {
    this->directed = isDirected;
}

Edge::~Edge() = default;
