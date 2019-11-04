//
// Created by cuongbv on 14/10/2019.
//

#include "Vertice.h"
#include "../utils/VerticeUtils.h"

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

std::vector<std::set<Vertice> > Vertice::getLocality() {
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
    for (int i = 0; i < Vertice::deltaNeighbors; ++i) {
        std::set<Vertice> higherNeighbors;
        for (auto &lowerNeighbor : this->locality[i]) {
            for (const auto &neighbor : lowerNeighbor.locality[0]) {
                if (this->locality[i].count(neighbor) > 0) {
                    higherNeighbors.insert(neighbor);
                }
            }
        }
        this->locality.push_back(higherNeighbors);
    }
    //Discover Block
    int maxGridHop = VerticeUtils::getMaxHopinBlock(this->verticeId, Vertice::xBlockSize, Vertice::yBlockSize,
                                                    Vertice::xTopoSize, Vertice::yTopoSize);
    for (int i = Vertice::deltaNeighbors; i < maxGridHop; ++i) {
        std::set<Vertice> higherNeighbors;
        for (auto &lowerNeighbor : this->locality[i]) {
            for (const auto &neighbor : lowerNeighbor.locality[0]) {
                if (this->locality[i].count(neighbor) > 0) {
                    int itsBlock = VerticeUtils::getVerticeBlock(this->verticeId, Vertice::xBlockSize, Vertice::yBlockSize, Vertice::xTopoSize, Vertice::yTopoSize);
                    int neighborBlock = VerticeUtils::getVerticeBlock(neighbor.)
                    higherNeighbors.insert(neighbor);
                }
            }
        }
    }
}

Vertice::~Vertice() = default;