//
// Created by cuongbv on 14/10/2019.
//

#include "Edge.h"
#include "../utils/VerticeUtils.h"
#include "Topo.h"


Edge::Edge(std::pair<int, int> verticesID) {
    this->points = verticesID;

    int firstHorizontal = VerticeUtils::getVerticeHorizontal(this->points.first, Topo::getXTopoSize());
    int firstVertical = VerticeUtils::getVerticeVertical(this->points.first, Topo::getYTopoSize());
    int secondHorizontal = VerticeUtils::getVerticeHorizontal(this->points.second, Topo::getXTopoSize());
    int secondVertical = VerticeUtils::getVerticeVertical(this->points.second, Topo::getYTopoSize());
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
