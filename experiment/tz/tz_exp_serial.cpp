//
// Created by cuongbv on 24/03/2020.
//

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../../node/TZNode.h"

int main() {
    std::map<int, TZNode*> tzNodeList;

    std::map<int, std::vector<std::pair<int, int> > > clusterTableMap;      // <sourceN, [destN, nextN]>
    std::map<int, std::vector<std::pair<int, int> > > landmarkTableMap;
    std::map<int, int> closetLandmarkMap;       // <sourceN, closetLandmarkN>
    std::map<std::pair<int, int>, std::vector<int> > allPaths;

    std::fstream clusterTableFile;
    std::fstream landmarkTableFile;
    std::fstream closetLandmarksFile;
    std::string lineString;

    clusterTableFile.open("./../experiment/tz/cluster_32x32", std::ios::in);
    while (!clusterTableFile.eof()) {
        std::getline(clusterTableFile, lineString);
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int destNodeID = std::stoi(lineVector[1]);
        int nextNodeID = std::stoi(lineVector[2]);
        clusterTableMap[sourceNodeID].push_back(std::pair<int, int>(destNodeID, nextNodeID));
        lineVector.clear();
    }
    clusterTableFile.close();

    landmarkTableFile.open("./../experiment/tz/landmark_32x32", std::ios::in);
    while (!landmarkTableFile.eof()) {
        std::getline(landmarkTableFile, lineString);
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int destLandmarkID = std::stoi(lineVector[1]);
        int nextNodeID = std::stoi(lineVector[2]);
        landmarkTableMap[sourceNodeID].push_back(std::pair<int, int> (destLandmarkID, nextNodeID));
        lineVector.clear();
    }
    landmarkTableFile.close();

    closetLandmarksFile.open("./../experiment/tz/closet_32x32", std::ios::in);
    while (!closetLandmarksFile.eof()) {
        std::getline(closetLandmarksFile, lineString);
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int closetLandmark = std::stoi(lineVector[1]);
        closetLandmarkMap.insert(std::pair<int, int>(sourceNodeID, closetLandmark));
        lineVector.clear();
    }
    closetLandmarksFile.close();

    int xTopoSize = 32;
    int yTopoSize = 32;
    int topoSize = xTopoSize * yTopoSize;

    // create TZ node list
    for (std::pair<int, int> tz_NodeID_closetLandmark : closetLandmarkMap) {
        auto *tzNode = new TZNode(tz_NodeID_closetLandmark.first);
        tzNode->setClosetLandmark(tz_NodeID_closetLandmark.second);
        tzNodeList.insert(std::pair<int, TZNode*>(tz_NodeID_closetLandmark.first, tzNode));
    }

    // update cluster routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceCluster : clusterTableMap) {
        for (std::pair<int, int> innerClusterNode : sourceCluster.second) {
            int sourceNodeID = sourceCluster.first;
            int destNodeID = innerClusterNode.first;
            int nextNodeID = innerClusterNode.second;
            tzNodeList[sourceNodeID]->updateClusterRT(destNodeID, nextNodeID);
        }
    }

    // update landmark routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceNode : landmarkTableMap) {
        for (std::pair<int, int> landmark : sourceNode.second) {
            int sourceNodeID = sourceNode.first;
            int destLandmarkID = landmark.first;
            int nextNodeID = landmark.second;
            tzNodeList[sourceNodeID]->updateLandmarkRT(destLandmarkID, nextNodeID);
        }
    }

    // routing for all pairs; this implementation is really more f#*kin' complex than its algorithm
    /* 0: not a landmark; 1: is a landmark => (source, dest) = {00, 01, 10, 11}
     *
     * 00:  dest in Cluster of source: (dest in C(source)): routing inner cluster: source --> dest
     *      dest is not in C(source): routing to the landmark of dest: source --> Landmark(dest) <-- dest
     * 01:  dest is the landmark of source: dest = Landmark(source): source --> Landmark(source) = dest
     *      dest != Landmark(source): routing to the dest: source --> Landmark(dest) = dest
     *      => but! always is source --> dest (via Landmark routing table)
     * 10:  source is the landmark of dest: source = Landmark(dest): source = Landmark(dest) <-- dest
     *      source != Landmark(dest): routing to the landmark of dest: source --> Landmark(dest) <-- dest
     * 11: the most easiest: source = Landmark(source) --> Landmark(dest) = dest
     */
    for (int sourceNodeID = 0; sourceNodeID < topoSize - 1; ++sourceNodeID) {
        for (int destNodeID = sourceNodeID + 1; destNodeID < topoSize; ++destNodeID) {
            std::vector<int> path;
            // true routing
            // if 11: both of sourceNodeID and destNodeID are landmark
            if (tzNodeList[sourceNodeID]->getClosetLandmark() == sourceNodeID) {
                if (tzNodeList[destNodeID]->getClosetLandmark() == destNodeID) {
                    path.push_back(sourceNodeID);
                    // routing: sourceNodeID --> L(destNodeID) = destNodeID
                    int nextNodeID = tzNodeList[sourceNodeID]->getLandmarkRT()[destNodeID];
                    while (nextNodeID != destNodeID) {
                        path.push_back(nextNodeID);
                        nextNodeID = tzNodeList[nextNodeID]->getLandmarkRT()[destNodeID];
                    }
                    path.push_back(destNodeID);
                    continue;
                }
            }
            // if 00
            if (tzNodeList[sourceNodeID]->getClosetLandmark() != sourceNodeID) {
                if (tzNodeList[destNodeID]->getClosetLandmark() != destNodeID) {
                    std::map<int, int> sourceClusterRT = tzNodeList[sourceNodeID]->getClusterRT();
                    // if dest in Cluster(source): routing inner cluster: source --> dest
                    if (sourceClusterRT.find(destNodeID) != sourceClusterRT.end()) {
                        path.push_back(sourceNodeID);
                        int nextNodeID = sourceClusterRT[destNodeID];
                        while (nextNodeID != destNodeID) {
                            path.push_back(nextNodeID);
                            nextNodeID = tzNodeList[nextNodeID]->getClusterRT()[destNodeID];    // Prove below:
                            /* Có thể tìm nextNodeID của nextNodeID thông qua tra cứu trong Cluster của nextNodeID
                             * Vì destNodeID sẽ luôn có mặt trong Cluster của nextNodeID.
                             * Chứng minh:
                             * Vì destNodeID thuộc Cluster của sourceNodeID
                             * Nên destNodeID gần sourceNodeID hơn Closet Landmark của chính nó (destNodeID)
                             *      destNodeID \in C(sourceNodeID) => d(sourceNodeID, destNodeID) < d(destNodeID, L(destNodeID))
                             * Giả sử có 1 điểm tempNodeID nằm trên Dijkstra shortest path d(sourceNodeID, destNodeID)
                             *      tempNodeID \in d(sourceNodeID, destNodeID)
                             * Khi đó, tempNodeID sẽ gần destNodeID hơn sourceNodeID
                             *      => d(tempNodeID, destNodeID) < d(sourceNodeID, destNodeID)
                             * Cho nên, destNodeID lại càng gần tempNodeID hơn Closet Landmark của chính nó (destNodeID)
                             *      => d(tempNodeID, destNodeID) < d(destNodeID, L(destNodeID))
                             * Do vậy, destNodeID sẽ nằm trong Cluster của tempNodeID
                             * Và ta có thể tìm nextNodeID tiếp theo thông qua Cluster của các nextNodeID trước đó
                             */
                        }
                        path.push_back(destNodeID);
                        continue;
                    } else {        // dest is not in Cluster(source): source --> Landmark(dest) <-- dest
                        // source --> Landmark(dest)
                        path.push_back(sourceNodeID);
                        int destLandmark = tzNodeList[destNodeID]->getClosetLandmark();
                        int nextNodeID = tzNodeList[sourceNodeID]->getLandmarkRT()[destLandmark];
                        while (nextNodeID != destLandmark) {
                            path.push_back(nextNodeID);
                            nextNodeID = tzNodeList[nextNodeID]->getLandmarkRT()[destLandmark];
                        }
                        path.push_back(destLandmark);       // add Landmark(dest) already
                        // Landmark(dest) <-- dest
                        std::vector<int> revertPath;
                        revertPath.push_back(destNodeID);
                        int revertNextNodeID = tzNodeList[destNodeID]->getLandmarkRT()[destLandmark];
                        while (revertNextNodeID != destLandmark) {
                            revertPath.push_back(revertNextNodeID);
                            revertNextNodeID = tzNodeList[revertNextNodeID]->getLandmarkRT()[destLandmark];
                        }
                        // combination source --> Landmark(dest) <-- dest
                        for (auto reverseNode = revertPath.rbegin(); reverseNode != revertPath.rend(); ++reverseNode) {
                            path.push_back(*reverseNode);
                        }
                        continue;
                    }
                }
            }
            // if 01
            if (tzNodeList[sourceNodeID]->getClosetLandmark() != sourceNodeID) {
                if (tzNodeList[destNodeID]->getClosetLandmark() == destNodeID) {
                    path.push_back(sourceNodeID);
                    int nextNodeID = tzNodeList[sourceNodeID]->getLandmarkRT()[destNodeID];
                    while (nextNodeID != destNodeID) {
                        path.push_back(nextNodeID);
                        nextNodeID = tzNodeList[nextNodeID]->getLandmarkRT()[destNodeID];
                    }
                    path.push_back(destNodeID);
                    continue;
                }
            }
            // if 10
            if (tzNodeList[sourceNodeID]->getClosetLandmark() == sourceNodeID) {
                if (tzNodeList[destNodeID]->getClosetLandmark() != destNodeID) {
                    // if source = Landmark(dest): source = Landmark(dest) <-- dest
                    if (tzNodeList[destNodeID]->getClosetLandmark() == sourceNodeID) {
                        // find revert path: source <-- dest
                        std::vector<int> revertPath;
                        revertPath.push_back(destNodeID);
                        int revertNextNodeID = tzNodeList[destNodeID]->getLandmarkRT()[sourceNodeID];
                        while (revertNextNodeID != sourceNodeID) {
                            revertPath.push_back(revertNextNodeID);
                            revertNextNodeID = tzNodeList[revertNextNodeID]->getLandmarkRT()[sourceNodeID];
                        }
                        revertPath.push_back(sourceNodeID);
                        // revert path: source --> dest
                        for (auto revertNode = revertPath.rbegin(); revertNode != revertPath.rend(); ++revertNode) {
                            path.push_back(*revertNode);
                        }
                        continue;
                    } else {    // source != Landmark(dest): source --> Landmark(dest) <-- dest
                        // source --> Landmark(dest)
                        path.push_back(sourceNodeID);
                        int destLandmark = tzNodeList[destNodeID]->getClosetLandmark();
                        int nextNodeID = tzNodeList[sourceNodeID]->getLandmarkRT()[destLandmark];
                        while (nextNodeID != destLandmark) {
                            path.push_back(nextNodeID);
                            nextNodeID = tzNodeList[nextNodeID]->getLandmarkRT()[destLandmark];
                        }
                        path.push_back(destLandmark);       // add Landmark(dest) already
                        // Landmark(dest) <-- dest
                        std::vector<int> revertPath;
                        revertPath.push_back(destNodeID);
                        int revertNextNodeID = tzNodeList[destNodeID]->getLandmarkRT()[destLandmark];
                        while (revertNextNodeID != destLandmark) {
                            revertPath.push_back(revertNextNodeID);
                            revertNextNodeID = tzNodeList[revertNextNodeID]->getLandmarkRT()[destLandmark];
                        }
                        // combination source --> Landmark(dest) <-- dest
                        for (auto reverseNode = revertPath.rbegin(); reverseNode != revertPath.rend(); ++reverseNode) {
                            path.push_back(*reverseNode);
                        }
                        continue;
                    }
                }
            }
            // add path to all-pair path
            std::pair<int, int> source_dest (sourceNodeID, destNodeID);
            allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> >(source_dest, path));
        }
    }
    // counting diameter and average-path-length
    int diameter = 0;
    int totalPathLength = 0;
    for (std::pair<std::pair<int, int>, std::vector<int> > path : allPaths) {
        if (path.second.size() > diameter) diameter = path.second.size();
        totalPathLength += path.second.size();
    }
    auto averagePathLength = (double) (totalPathLength / allPaths.size());
    std::cout << "Diameter: " << diameter << std::endl;
    std::cout << "Average Path Length: " << averagePathLength << std::endl;
}