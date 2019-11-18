//
// Created by cuongbv on 13/11/2019.
//

#ifndef PARALLEL_ROUTING_TOPO_H
#define PARALLEL_ROUTING_TOPO_H

#include "Graph.h"

class Topo : public Graph {
private:
    // static global parameters of Topology

public:
//    Topo(int xTopoSize, int yTopoSize, int XBlockSize, int yBlockSize, int deltaNeighbors);
    static int xTopoSize;
    static int yTopoSize;
    static int xBlockSize;
    static int yBlockSize;
    static int deltaNeighbors;
    static float nodeLatency;
    static float cableLatency;
    // Getter
    static int getXTopoSize();
    static int getYTopoSize();
    static int getXBlockSize();
    static int getYBlockSize();
    static int getDeltaNeighbors();

    // Setter
    static void setNodeLatency(float latency);
    static void setcableLatency(float latency);

};


#endif //PARALLEL_ROUTING_TOPO_H
