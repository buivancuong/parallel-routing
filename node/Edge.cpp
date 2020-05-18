//
// Created by cuongbv on 22/12/2019.
//

#include "Edge.h"
#include "../utils/CORRAUtils.h"


Edge::Edge(std::pair<int, int> verticesID, int xTopoSize, int yTopoSize) {
    this->points = verticesID;

    int firstHorizontal = CORRAUtils::getNodeHorizontal(this->points.first, xTopoSize);
    int firstVertical = CORRAUtils::getNodeVertical(this->points.first, yTopoSize);
    int secondHorizontal = CORRAUtils::getNodeHorizontal(this->points.second, xTopoSize);
    int secondVertical = CORRAUtils::getNodeVertical(this->points.second, yTopoSize);
    this->weight = abs(firstHorizontal - secondHorizontal) + abs(firstVertical - secondVertical);
}

double Edge::getWeight() {
    return this->weight;
}


void Edge::setWeight(double newWeight) {
    this->weight = newWeight;
}


bool Edge::isExistEdges(const Edge& firstEdge, const Edge& secondEdge) {
    if (firstEdge.points.first == secondEdge.points.second) {
        return firstEdge.points.second == secondEdge.points.first;
    } else return false;
}

Edge::Edge(Edge *edge) {
    this->points = edge->getPoints();
    this->weight = edge->weight;
}

std::pair<int, int> Edge::getPoints() {
    return this->points;
}


Edge::~Edge() = default;