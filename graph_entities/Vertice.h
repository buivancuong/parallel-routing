//
// Created by cuongbv on 14/10/2019.
//

#ifndef PARALLEL_ROUTING_VERTICE_H
#define PARALLEL_ROUTING_VERTICE_H

#include <string>
#include <vector>
#include <map>
#include <list>

class Vertice {

protected:
    int vertice_id{};       // id of vertice
    bool isCenter{};      // isCenter=True if vertice is center of its block
    std::string vertice_role;       // role of vertice {switch, router, terminal computer, ...}
    // below variable is stepwise updated, no have Setter function
    std::vector<std::vector<Vertice> > locality;        // locality of this vertice
    std::map<int, int> localRT;     // Routing table to node on locality.
    std::vector<std::list<int> > blockRT;       // Routing table to blocks


public:
    // Constructer & Destructer
    Vertice();
    virtual ~Vertice();

    // Getter methods
    int getVerticeID();
    bool getCentered();
    std::string getVerticeRole();
    std::vector<std::vector<Vertice> > getLocality();
    std::map<int, int> getLocalRT();
    std::vector<std::list<int> > getBlockRT();

    // Setter methods
    void setVerticeID(int newID);
    void setCentered(bool centered);
    void setVerticeRole(std::string newRole);

    // Stepwise update methods
    void createLocality();
    void updateLocalRT();
    void updateLocalTable(int destN, int next);
    void getTreeSPR();

    void updateBlockRT(int destB, int next);
    void find1Bridges();
    void find2Bridges();
    void find3Bridges();
    void find4Bridges();

    void handleMissingBridge

};


#endif //PARALLEL_ROUTING_VERTICE_H
