//
// Created by cuongbv on 24/03/2020.
//

#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include "../../node/TZNode.h"

int main() {
    std::map<int, TZNode*> tzNodeList;

    std::vector<std::pair<int, std::pair<int, int> > > clusterTableVector;
    std::vector<std::pair<int, std::pair<int, int> > > landmarkTableVector;
    std::vector<std::pair<int, int> > closetLandmarkVector;
    std::map<std::pair<int, int>, std::vector<int> > allPaths;

    std::fstream clusterTableFile;
    std::fstream landmarkTableFile;
    std::fstream closetLandmakrsFile;
    std::string lineString;

    clusterTableFile.open("./../experiment/tz/local_32x32", std::ios::in);
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
        std::pair<int, int> dest_next(destNodeID, nextNodeID);
        std::pair<int, std::pair<int, int> > source_dest(sourceNodeID, dest_next);
        clusterTableVector.push_back(source_dest);
        lineVector.clear();
    }
    clusterTableFile.close();

    landmarkTableFile.open("./../experiment/tz/block_32x32", std::ios::in);
    while (!landmarkTableFile.eof()) {
        std::getline(landmarkTableFile, lineString);
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int desBlockID = std::stoi(lineVector[1]);
        int nextNodeID = std::stoi(lineVector[2]);
        std::pair<int, int> destB_nextN(desBlockID, nextNodeID);
        std::pair<int, std::pair<int, int> > sourceN_destB(sourceNodeID, destB_nextN);
        landmarkTableVector.push_back(sourceN_destB);
        lineVector.clear();
    }
    landmarkTableFile.close();

    closetLandmakrsFile.open("./../experiment/tz/closet_32x32", std::ios::in);
    while (!closetLandmakrsFile.eof()) {
        std::getline(closetLandmakrsFile, lineString);
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int closetLandmark = std::stoi(lineVector[1]);
        closetLandmarkVector.emplace_back(sourceNodeID, closetLandmark);
        lineVector.clear();
    }
    closetLandmakrsFile.close();

    int topoSize = clusterTableVector.back().first + 1;       // from ID = 0
    int xTopoSize, yTopoSize;
    if ((int)(log2(topoSize)) % 2 == 0) {
        xTopoSize =  (int) sqrt(topoSize);
        yTopoSize = (int) sqrt(topoSize);
    } else {
        xTopoSize = (int) sqrt(topoSize / 2);
        yTopoSize = (int) (topoSize / xTopoSize);
    }

    // create TZ node list
    for (std::pair<int, int> tz_NodeID_closetLandmark : closetLandmarkVector) {
        auto *tzNode = new TZNode(tz_NodeID_closetLandmark.first);
        tzNode->setClosetLandmark(tz_NodeID_closetLandmark.second);
        tzNodeList.insert(std::pair<int, TZNode*>(tz_NodeID_closetLandmark.first, tzNode));
    }

    // update local routing table
    for (std::pair<int, std::pair<int, int> > clusterRecord : clusterTableVector) {
        int sourceNodeID = clusterRecord.first;
        int destNodeID = clusterRecord.second.first;
        int nextNodeID = clusterRecord.second.second;
        tzNodeList[sourceNodeID]->updateClusterRT(destNodeID, nextNodeID);
    }

    // update block routing table
    for (std::pair<int, std::pair<int, int> > landmarkRecord : landmarkTableVector) {
        int sourceNodeID = landmarkRecord.first;
        int destBlockID = landmarkRecord.second.first;
        int nextNodeID = landmarkRecord.second.second;
        tzNodeList[sourceNodeID]->updateLandmarkRT(destBlockID, nextNodeID);
    }

    // routing for all pairs; this is really more f#*kin' complex than its algorithm
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