//
// Created by cuongbv on 14/10/2019.
//

#include "Vertice.h"
#include "../utils/VerticeUtils.h"
#include "Topo.h"
#include "Graph.h"

#include <utility>

Vertice::Vertice(int verticeId) {
    this->verticeId = verticeId;
}


int Vertice::getVerticeID() {
    return this->verticeId;
}

void Vertice::setVerticeID(int newID) {
    this->verticeId = newID;
}

bool Vertice::getCentered() {
    return this->isCenter;
}

std::vector<std::map<int, Vertice> > Vertice::getLocality() {
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
    for (int i = 0; i < Topo::getDeltaNeighbors() - 1; ++i) {       // counting from 0, not 1
        std::map<int, Vertice> higherNeighbors;
        for (std::pair<int, Vertice> lowerNeighbor : this->locality[i]) {       // for each vertice on lower neighbor layer (current neighbor)
            for (std::pair<int, Vertice> neighbor : lowerNeighbor.second.getLocality()[0]) {     // for each neighbor of each above vertice (new neighbor)
                if (neighbor.first == this->getVerticeID()) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first) != 0) continue;       // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first != 0)))
                    continue;       // neighbor is existed at inner layer neighbor
                if (higherNeighbors.count(neighbor.first) ==
                    0) {     // if new higher neighbor layer is not include new neighbor (
                    int itsBlock = VerticeUtils::getVerticeBlock(this->verticeId, Topo::getXBlockSize(),
                                                                 Topo::getYBlockSize(), Topo::getXTopoSize(),
                                                                 Topo::getYTopoSize());
                    int neighborBlock = VerticeUtils::getVerticeBlock(neighbor.second.verticeId, Topo::getXBlockSize(),
                                                                      Topo::getYBlockSize(), Topo::getXTopoSize(),
                                                                      Topo::getYTopoSize());
                    if (itsBlock == neighborBlock) {
                        higherNeighbors.insert(std::pair<int, Vertice>(neighbor.second.getVerticeID(),
                                                                       neighbor.second));       // add it into new higher layer neighbor
                    }
                }
            }
        }
        this->locality.push_back(
                higherNeighbors);      // push new higher layer neighbor back to locality vector (array)
    }
    //Discover Block \ Neighbors
    int maxGridHop = VerticeUtils::getMaxHopinBlock(this->verticeId, Topo::getXBlockSize(),
                                                    Topo::getYBlockSize(), Topo::getXTopoSize(),
                                                    Topo::getYTopoSize());
    for (int i = Topo::getDeltaNeighbors() - 1; i < maxGridHop - 1; ++i) {
        std::map<int, Vertice> higherNeighbors;
        for (std::pair<int, Vertice> lowerNeighbor : this->locality[i]) {
            for (std::pair<int, Vertice> neighbor : lowerNeighbor.second.getLocality()[0]) {     // 2 layer reference is not enable to extract datatype
                if (neighbor.first == this->getVerticeID()) continue;       // neighbor of neighbor is self
                if (higherNeighbors.count(neighbor.first) != 0) continue;       // mutual neighbor
                if ((i > 0) and (this->locality[i - 1].count(neighbor.first != 0)))
                    continue;       // neighbor is existed at inner layer neighbor
                if (higherNeighbors.count(neighbor.first) ==
                    0) {     // if new higher neighbor layer is not include new neighbor (
                    int itsBlock = VerticeUtils::getVerticeBlock(this->verticeId, Topo::getXBlockSize(),
                                                                 Topo::getYBlockSize(), Topo::getXTopoSize(),
                                                                 Topo::getYTopoSize());
                    int neighborBlock = VerticeUtils::getVerticeBlock(neighbor.second.verticeId, Topo::getXBlockSize(),
                                                                      Topo::getYBlockSize(), Topo::getXTopoSize(),
                                                                      Topo::getYTopoSize());
                    if (itsBlock == neighborBlock) {
                        higherNeighbors.insert(std::pair<int, Vertice>(neighbor.second.getVerticeID(),
                                                                       neighbor.second));       // add it into new higher layer neighbor
                    }
                }
            }
        }
        this->locality.push_back(higherNeighbors);
    }
}

void Vertice::updateLocalRT() {
    //
}

void Vertice::addNeighbor(Vertice neighborVertice) {
    if (this->locality[0].count(neighborVertice.getVerticeID()) == 0) {
        this->locality[0].insert(std::pair<int, Vertice>(neighborVertice.getVerticeID(), neighborVertice));
    }
}


Vertice::~Vertice() = default;