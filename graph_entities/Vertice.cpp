//
// Created by cuongbv on 14/10/2019.
//

#include "Vertice.h"

#include <utility>

Vertice::Vertice(int verticeId) {
    this->verticeId = verticeId;
}


int Vertice::getVerticeID() {
    return this->verticeId;
}

std::string Vertice::getVerticeRole() {
    return this->verticeRole;
}

void Vertice::setVerticeID(int newID) {
    this->verticeId = newID;
}

void Vertice::setVerticeRole(std::string newRole) {
    this->verticeRole = std::move(newRole);
}

bool Vertice::getCentered() {
    return this->isCenter;
}

std::vector<std::vector<Vertice> > Vertice::getLocality() {
    return this->locality;
}

std::map<int, int> Vertice::getLocalRT() {
    return this->localRT;
}

std::vector<std::list<int> > Vertice::getBlockRT() {
    return this->blockRT;
}

void Vertice::setCentered(bool centered) {
    this->isCenter = centered;
}

// Locality = Neighbors + Block
void Vertice::createLocality() {
    // Discover Neighbors
    for (int i = 0; i < Vertice::deltaNeighbors;  ++i) {
        std::vector<Vertice> higherNeighbors;
        for (auto &lowerNeighbor : this->locality[i]) {
            for (auto neighbor : lowerNeighbor.locality[0]) {
                higherNeighbors.push_back(lowerNeighbor);
            }
        }
        this->locality.push_back(higherNeighbors);
    }
    //Discover Block
    while (true){

    }
}

Vertice::~Vertice() = default;