//
// Created by cuongbv on 14/10/2019.
//

#include "Edge.h"


//Edge::Edge(int *verticesID) {
//    this->vertices[0] = Vertice(verticesID[0]);
//    this->vertices[1] = Vertice(verticesID[1]);
//}

float Edge::getWeight() {
    return this->weight;
}

bool Edge::getDirected() {
    return this->directed;
}

Edge::~Edge() = default;
