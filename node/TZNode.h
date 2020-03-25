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
    int closetLandmark{};
    std::map<int, std::pair<float, int> > traceMap;
    std::map<int, TZNode*> cluster;
    std::map<int, int> clusterRT;
    std::map<int, int> landmarkRT;

public:

    TZNode();
    explicit TZNode(int nodeID);
    ~TZNode();

    int getNodeID();
    int getClosetLandmark();
    std::map<int, std::pair<float, int> > getTraceMap();
    std::map<int, TZNode*> getCluster();
    void setClosetLandmark(int landmarkNodeID);
    std::map<int, int> getClusterRT();
    std::map<int, int> getLandmarkRT();
    void updateClusterRT(int destNodeID, int nextNodeID);
    void updateLandmarkRT(int destLandmarkNodeID, int nextNodeID);


    // first, create traceMap to get single source Dijkstra shortest path from this->nodeID
    void createTraceMap(Graph *globalGraph);
    // extract closetLandmark
    void extractClosetLandmark(std::map<int, TZNode*> landmarks);
    // extract cluster set of this->nodeID
    void extractCluster(const std::map<int, TZNode*>& nodeList);
    // create routing table on own cluster
    void createClusterRT();
    // create routing table to all of landmarks
    void createLandmarkRT(const std::map<int, TZNode*>& landmarkSet);

};


#endif //PARALLEL_ROUTING_TZNODE_H
