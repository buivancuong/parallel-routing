#include <iostream>
#include <utility>
#include "./utils/VerticeUtils.h"
#include "./graph_entities/Edge.h"
#include "graph_entities/Topo.h"

int Topo::xTopoSize = 1;
int Topo::yTopoSize = 1;
int Topo::xBlockSize = 1;
int Topo::yBlockSize = 1;
int Topo::deltaNeighbors = 1;
float Topo::nodeLatency = 1;
float Topo::cableLatency = 1;
int main() {


    auto *vertice1 = new Vertice(1);
    auto *vertice2 = new Vertice(2);

    std::cout << vertice1->getVerticeID() << std::endl;
    return 0;
}