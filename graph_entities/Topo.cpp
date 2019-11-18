//
// Created by cuongbv on 13/11/2019.
//

#include "Topo.h"

int Topo::getXTopoSize() {
    return Topo::xTopoSize;
}

int Topo::getYTopoSize() {
    return Topo::yTopoSize;
}

int Topo::getXBlockSize() {
    return Topo::xBlockSize;
}

int Topo::getYBlockSize() {
    return Topo::yBlockSize;
}

int Topo::getDeltaNeighbors() {
    return Topo::deltaNeighbors;
}

void Topo::setNodeLatency(float latency) {
    Topo::nodeLatency = latency;
}

void Topo::setcableLatency(float latency) {
    Topo::cableLatency = latency;
}

//Topo::Topo(int xTopoSize, int yTopoSize, int XBlockSize, int yBlockSize, int deltaNeighbors) {
//    Topo::xTopoSize = xTopoSize;
//    Topo::yTopoSize = yTopoSize;
//    Topo::xBlockSize = xBlockSize;
//    Topo::yBlockSize = yBlockSize;
//    Topo::deltaNeighbors = deltaNeighbors;
//}

