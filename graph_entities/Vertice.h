//
// Created by cuongbv on 14/10/2019.
//

#ifndef PARALLEL_ROUTING_VERTICE_H
#define PARALLEL_ROUTING_VERTICE_H

#include <string>
#include <vector>
#include <map>
#include <list>
#include <set>

//#include "Graph.h"

class Vertice {

protected:
    int verticeId{};       // id of vertice
    bool isCenter{};      // isCenter=True if vertice is center of its block
    // below variable is stepwise updated, no have Setter function
    std::vector<std::map<int, Vertice> > locality;        // locality of this vertice:
    std::map<int, int> localRT;     // Routing table to node on locality.
    std::vector<std::list<int> > blockRT;       // Routing table to blocks

public:
    // Constructer & Destructer
    explicit Vertice(int verticeId);
    virtual ~Vertice();

    // Getter methods
    int getVerticeID();
    bool getCentered();
    std::vector<std::map<int, Vertice> > getLocality();
    std::map<int, int> getLocalRT();
    std::vector<std::list<int> > getBlockRT();

    // Setter methods
    void setVerticeID(int newID);
    void setCentered(bool centered);
    void addNeighbor(Vertice neighborVertice);

    // Stepwise update methods
    void createLocality();
    void updateLocalRT();
    void updateLocalTable(int destN, int next);

    void updateBlockRT(int destB, int next);
    void find1Bridges();
    void find2Bridges();
    void find3Bridges();
    void find4Bridges();

    void handleMissingBridge();

};


#endif //PARALLEL_ROUTING_VERTICE_H
