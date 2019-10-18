//
// Created by cuongbv on 14/10/2019.
//

#include "Edge.h"


Edge::Edge(Vertice *vertices) {
    this->vertices[0] = vertices[0];
    this->vertices[1] = vertices[1];
}

float Edge::getWeight() {
    return this->weight;
}

bool Edge::getDirected() {
    return this->directed;
}

Edge::~Edge() = default;
