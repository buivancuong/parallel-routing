//
// Created by cuongbv on 17/02/2020.
//

#ifndef PARALLEL_ROUTING_TZNODE_H
#define PARALLEL_ROUTING_TZNODE_H

#include <map>
#include "../graph/Graph.h"


class TZNode {
private:
    int nodeID{};
    bool isLandmark{};
    int closetLandmark{};
    std::map<int, std::pair<float, int> > traceMap;
    std::map<int, TZNode*> cluster;


public:
    static std::map<int, TZNode*> *landmarks;
    static std::map<int, TZNode*> *nodeList;

    TZNode();
    explicit TZNode(int nodeID);
    ~TZNode();

    int getNodeID();
    int getClosetLandmark();
    std::map<int, std::pair<float, int> > getTraceMap();
    bool isLandmarkNode();
    void setIsLandMark();
    void setClosetLandmark(int landmarkNodeID);


    // first, create traceMap to get single source Dijkstra shortest path from this->nodeID
    void createTraceMap(Graph *globalGraph);
    // extract closetLandmark
    void extractClosetLandmark();
    // extract cluster set of this->nodeID
    void extractCluster();

};


#endif //PARALLEL_ROUTING_TZNODE_H
