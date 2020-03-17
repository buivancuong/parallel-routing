//
// Created by cuongbv on 20/12/2019.
//

#ifndef PARALLEL_ROUTING_NODE_H
#define PARALLEL_ROUTING_NODE_H


#include <map>
#include <vector>

class Node {
private:
    int nodeID{};
    bool isCenterNode{};
    std::map<int, Node*> nearNeighbors;     // grid neighbors
    std::map<int, Node*> farNeighbors;      // random link neighbors
    std::vector<std::map<int, Node*> > locality;
    std::map<int, std::pair<int, double> > localRT;
    std::map<int, std::pair<double, int> > traceMap;        // <nodeID, <distance, prevNodeID> >
    std::map<int, int> bridgeRT;        // <destBlockID, nextNodeID>
    std::map<int, std::vector<std::vector<std::pair<int, Node*> > > > bridgeList;        // B2B: <destBlockID, [bridge, bridge, ...]>, (bridge = [<int, Node>])
    std::vector<std::vector<std::pair<int, Node*> > > ownBridges;        // [bridge], bridge = [<int, Node>]

    void addOwnBridge(const std::vector<std::pair<int, Node*> >& newOwnBridge);
    static bool checkBridge(const std::vector<std::pair<int, Node*> >& bridge);
    static double getBridgeLatency(const std::vector<std::pair<int, Node*> >& bridge, int xTopoSize);
    std::pair<int, Node*> getCenterNode(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);

public:
    Node();
    explicit Node(int nodeID);
    ~Node();

    void setCentered(bool centered);
    bool getCentered();
    std::map<int, std::pair<int, double> > getLocalRT();
    std::map<int, int> getBlockRT();

    std::vector<std::map<int, Node*> > getLocality();
    std::map<int, std::vector<std::vector<std::pair<int, Node*> > > > getBridgeList();
    int getNodeID();
    std::map<int, Node*> getNearNeighbors();
    std::map<int, Node*> getFarNeighbors() const;
    void addNearNeighbors(Node *nearNeighbor);
    void addFarNeighbors(Node *farNeighbor);
    std::vector<std::vector<std::pair<int, Node*> > > getOwnBridges();
    void updateBridgeList(std::vector<std::pair<int, Node*> > bridge, int xBlockSize, int yBlockSize, int xTopoSize);
    std::vector<std::pair<int, Node*> > localShortestPath(int destID);

    // first, select neighbors in nearNeighbors and farNeighbors to put into first layer in locality
    void prepareLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSze, int yTopoSize);
    // next, from first layer of locality, each Node create own locality by itself
    void createLocality(int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    // next, after get correct locality, each Node update local routing table for the other Nodes in own locality
    void createLocalRouting(int xTopoSize);
    // this function supports update to own local routing table
    void updateLocalRT(int destNodeID, int nextNodeID, double latency);
    // next, find all BR1 from this Node (random links), save bridge to @ownBridges
    void findBR1();
    // next, find all BR2, BR3, ..., BRn, from this Node, save bridge to @ownBridges
    void findToBRn(int n);
    // next, after get all BRn (from this Node), broadcast bridges in @ownBridges to each other Node in onw @locality
    void broadcastLocalBridge(int xBlockSize, int yBlockSize, int xTopoSize);
    // next, after received all bridges that has broadcasted from the other Node, check missing brides status 
    void handleMissingBridge(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
    // this function supports broadcasting the found missing bridge to each other Node in own @locality
    void broadcastMissingBridge(const std::vector<std::pair<int, Node*> >& bridge, int xBlockSize, int yBlockSize, int xTopoSize);
    // at last, from full bridges list in @bridgeList, each Node update own block routing table 
    void updateBlockTable(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize);
};


#endif //PARALLEL_ROUTING_NODE_H
