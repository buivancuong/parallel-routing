//
// Created by cuongbv on 14/10/2019.
//

#include "Vertice.h"

#include <utility>

Vertice::Vertice() = default;


int Vertice::getVerticeID() {
    return this->vertice_id;
}

std::string Vertice::getVerticeRole() {
    return this->vertice_role;
}

void Vertice::setVerticeID(int newID) {
    this->vertice_id = newID;
}

void Vertice::setVerticeRole(std::string newRole) {
    this->vertice_role = std::move(newRole);
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

Vertice::~Vertice() = default;