//
// Created by cuongbv on 03/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRANODE_H
#define PARALLEL_ROUTING_CORRANODE_H


#include <vector>
#include <map>
#include "../graph/Graph.h"

class CORRANode {
private:
    int nodeID{};
    bool isCenterNode{};
    std::map<int, CORRANode*> nearNeighbors;
    std::map<int, CORRANode*> farNeighbors;
    std::vector<std::map<int, CORRANode*> > locality;
    std::map<int, std::pair<int, double> > localRT;
    std::map<int, std::pair<int, std::vector<int> > > blockRT;
    std::map<int, std::pair<double, int> > localTraceMap;
    std::map<int, std::pair<float, int> > globalTraceMap;
    std::map<int, std::vector<std::vector<std::pair<int, CORRANode*> > > > bridgeList;
    std::vector<std::vector<std::pair<int, CORRANode*> > > ownBridges;

    static bool checkBridge(const std::vector<std::pair<int, CORRANode*> >& bridge);
    static int getBridgeCost(const std::vector<std::pair<int, CORRANode *> >& bridge, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);


public:
    CORRANode();
    explicit CORRANode(int nodeID);
    ~CORRANode();

    bool getCentered();
    void setCentered(bool centered);
    std::map<int, std::pair<int, double> > getLocalRT();
    std::map<int, std::pair<int, std::vector<int> > > getBlockRT();

    std::vector<std::map<int, CORRANode*> > getLocality();
    std::map<int, std::vector<std::vector<std::pair<int, CORRANode*> > > > getBridgeList();
    int getNodeID();
    std::map<int, std::pair<float, int> > getGlobalTraceMap();
    std::map<int, CORRANode*> getNearNeighbors();
    std::map<int, CORRANode*> getFarNeighbors();
    void addNearNeighbors(CORRANode *nearNeighbor);
    void addFarNeighbors(CORRANode *farNeighbor);
    void updateBridgeList(std::vector<std::pair<int, CORRANode*> > bridge, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    int getNextNodeID(int destID);
    std::vector<std::vector<std::pair<int, CORRANode*> > > getOwnBridges();
    void updateBlockRT(int destBlockID, int nextNodeID, const std::vector<int>& bridge);

    // first, select neighbors in nearNeighbors and farNeighbors to put into first layer in locality
    void prepareLocality(int deltaNeighbor, int xTopoSize);
    // next, from first layer of locality, each Node create own locality by itself
    void createLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize);
    // next, after get correct locality, each Node update local routing table for the other Nodes in own locality
    void createLocalRouting(int xTopoSize);
    // this function supports update to own local routing table
    void updateLocalRT(int destNodeID, int nextNodeID, double latency = 0);
    // next, find all BR1 from this Node (random links), save bridge to @ownBridges
    void findBR1();
    // next, find BR2 (n = 2) from this Node, save bridge to @ownBridges
    void findToBRn(int n = 2);
    // next, after get all BRn (from this Node), broadcast bridges in @ownBridges to each other Node in onw @locality
    void broadcastLocalBridge(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    // next, after received all bridges that has broadcast from the other Node, create this->globalTraceMap for missing bridges
    void createGlobalTraceMap(Graph *globalGraph);
    // last, update this->blockRT
    void updateBlockTable(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, std::map<int, CORRANode*> corra1NodeList);

};



#endif //PARALLEL_ROUTING_CORRANODE_H
