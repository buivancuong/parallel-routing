//
// Created by cuongbv on 03/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRANODE_H
#define PARALLEL_ROUTING_CORRANODE_H


#include <vector>
#include <map>

class CORRANode {
private:
    int nodeID{};
    std::map<int, CORRANode*> nearNeighbors;
    std::map<int, CORRANode*> farNeighbors;
    std::vector<std::map<int, CORRANode*> > locality;
    std::map<int, std::pair<int, double> > routingTable;
    std::map<int, std::pair<double, int> > traceMap;
    std::map<int, std::vector<std::vector<std::pair<int, CORRANode*> > > > bridgeList;
    std::vector<std::vector<std::pair<int, CORRANode*> > > ownBridges;

    static bool checkBridge(const std::vector<std::pair<int, CORRANode*> >& bridge);


public:
    CORRANode();
    explicit CORRANode(int nodeID);
    ~CORRANode();

    std::vector<std::map<int, CORRANode*> > getLocality();
    std::map<int, std::vector<std::vector<std::pair<int, CORRANode*> > > > getBridgeList();
    int getNodeID();
    std::map<int, CORRANode*> getNearNeighbors();
    std::map<int, CORRANode*> getFarNeighbors();
    void addNearNeighbor(CORRANode *nearNeighbor);
    void addFarNeighbor(CORRANode *farNeighbor);


    void prepareLocality(int deltaNeighbor, int xTopoSize, int yTopoSize);
    void createLocality(int deltaNeighbor, int xTopoSize, int yTopoSize);
    void findBR1();
    void findBRn(int n = 2);

};



#endif //PARALLEL_ROUTING_CORRANODE_H
