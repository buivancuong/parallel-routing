//
// Created by cuongbv on 24/03/2020.
//

#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include "../../node/TZNode.h"

int main() {
    std::map<int, TZNode*> tzNodeList;

    std::vector<std::pair<int, std::pair<int, int> > > clusterTableVector;
    std::vector<std::pair<int, std::pair<int, int> > > landmarkTableVector;
    std::map<std::pair<int, int>, std::vector<int> > allPaths;

    std::fstream clusterTableFile;
    std::fstream landmarkTableFile;
    std::string lineString;

    clusterTableFile.open("./../experiment/corra_v1/local_32x32r4", std::ios::in);
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

    landmarkTableFile.open("./../experiment/corra_v1/block_32x32r4", std::ios::in);
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
    for (int i = 0; i < topoSize; ++i) {
        auto *tzNode = new TZNode(i);
        tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
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

    // routing with all pairs
    for (int sourceNodeID = 0; sourceNodeID < topoSize - 1; ++sourceNodeID) {
        for (int destNodeID = sourceNodeID + 1; destNodeID < topoSize; ++destNodeID) {
            std::vector<int> path;
            path.push_back(sourceNodeID);
            // true routing
            std::map<int, int> sourceClusterRT = tzNodeList[sourceNodeID]->getClusterRT();
            std::map<int, int> sourceLandmarkRT = tzNodeList[sourceNodeID]->getLandmarkRT();
            if (sourceClusterRT.find(destNodeID) != sourceClusterRT.end()) {        // routing in cluster
                int nextNodeID = sourceClusterRT[destNodeID];
                while (nextNodeID != destNodeID) {
                    path.push_back(nextNodeID);
                    nextNodeID = tzNodeList[nextNodeID]->getClusterRT()[destNodeID];
                    /* Có thể tìm nextNodeID của nextNodeID thông qua tra cứu trong Cluster của nextNodeID
                     * Vì destNodeID sẽ luôn có mặt trong Cluster của nextNodeID.
                     * Chứng minh:
                     * Vì destNodeID thuộc Cluster của sourceNodeID => destNodeID gần sourceNodeID hơn Closet Landmark của chính nó (destNodeID)
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
            } else {        // find path to landmark of destNodeID

            }
            // add the last node
            path.push_back(destNodeID);
            // add path to all-pair path
            std::pair<int, int> source_dest (sourceNodeID, destNodeID);
            allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> >(source_dest, path));
        }
    }

}