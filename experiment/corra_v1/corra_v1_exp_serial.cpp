//
// Created by cuongbv on 18/03/2020.
//

#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include "../../node/CORRANode.h"
#include "../../utils/CORRAUtils.h"


int main() {
    std::map<int, CORRANode*> corra1NodeList;

    std::vector<std::pair<int, std::pair<int, int> > > localTableVector;
    std::vector<std::pair<int, std::pair<int, int> > > blockTableVector;
    std::map<std::pair<int, int>, std::vector<int> > allPaths;       // < <source, dest>, [vector of path] >

    std::fstream localTableFile;
    std::fstream blockTableFile;
    std::string lineString;

    localTableFile.open("./../experiment/corra_v1/local_32x32r4", std::ios::in);
    while (!localTableFile.eof()) {
        std::getline(localTableFile, lineString);
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
        localTableVector.push_back(source_dest);
        lineVector.clear();
    }

    blockTableFile.open("./../experiment/corra_v1/block_32x32r4", std::ios::in);
    while (!blockTableFile.eof()) {
        std::getline(blockTableFile, lineString);
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
        blockTableVector.push_back(sourceN_destB);
        lineVector.clear();
    }

    int topoSize = localTableVector.back().first + 1;       // from ID = 0
    int xTopoSize, yTopoSize;
    if ((int)(log2(topoSize)) % 2 == 0) {
        xTopoSize =  (int) sqrt(topoSize);
        yTopoSize = (int) sqrt(topoSize);
    } else {
        xTopoSize = (int) sqrt(topoSize / 2);
        yTopoSize = (int) (topoSize / xTopoSize);
    }
    int xBlockSize, yBlockSize;
    if ((int) (log2(xTopoSize)) % 2 == 0) {
        xBlockSize = (int) sqrt(xTopoSize);
    } else {
        xBlockSize = (int) sqrt(xTopoSize / 2);
    }
    if ((int) (log2(yTopoSize)) % 2 == 0) {
        yBlockSize = (int) sqrt(yTopoSize);
    } else {
        yBlockSize = (int) sqrt(yTopoSize / 2);
    }

    // create CORRA v1 node list
    for (int i = 0; i < topoSize; ++i) {
        auto *corraNode = new CORRANode(i);
        corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
    }

    // update local routing table
    for (std::pair<int, std::pair<int, int> > localRecord : localTableVector) {
        int sourceNodeID = localRecord.first;
        int destNodeID = localRecord.second.first;
        int nextNodeID = localRecord.second.second;
        corra1NodeList[sourceNodeID]->updateLocalRT(destNodeID, nextNodeID);
    }

    // update block routing table
    for (std::pair<int, std::pair<int, int> > blockRecord : blockTableVector) {
        int sourceNodeID = blockRecord.first;
        int destBlockID = blockRecord.second.first;
        int nextNodeID = blockRecord.second.second;
        corra1NodeList[sourceNodeID]->updateBlockRT(destBlockID, nextNodeID);
    }

    // routing with all pairs
    for (int sourceNodeID = 0; sourceNodeID < topoSize - 1; ++sourceNodeID) {
        for (int destNodeID = sourceNodeID + 1; destNodeID < topoSize; ++destNodeID) {
            std::vector<int> path;
            path.push_back(sourceNodeID);
            // true routing
            std::map<int, std::pair<int, double> > sourceLocalRT = corra1NodeList[sourceNodeID]->getLocalRT();
            std::map<int, int> sourceBlockRT = corra1NodeList[sourceNodeID]->getBlockRT();
            if (sourceLocalRT.find(destNodeID) != sourceLocalRT.end()) {        // routing in local
                int nextNodeID = sourceLocalRT[destNodeID].first;
                while (nextNodeID != destNodeID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra1NodeList[nextNodeID]->getLocalRT()[destNodeID].first;
                }
            } else {        // routing beyond local
                int destBlockID = CORRAUtils::getNodeBlock(destNodeID, xBlockSize, yBlockSize, xTopoSize);
                int nextNodeID = sourceBlockRT[destBlockID];
                int nextBlockID = CORRAUtils::getNodeBlock(nextNodeID, xBlockSize, yBlockSize, xTopoSize);
                while (nextBlockID != destBlockID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra1NodeList[nextNodeID]->getBlockRT()[nextNodeID];
                    nextBlockID = CORRAUtils::getNodeBlock(nextBlockID, xBlockSize, yBlockSize, xTopoSize);
                }
                while (nextNodeID != destNodeID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra1NodeList[nextNodeID]->getLocalRT()[destNodeID].first;
                }
            }
            // add the last node
            path.push_back(destNodeID);
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