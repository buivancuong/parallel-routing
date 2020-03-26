//
// Created by cuongbv on 23/03/2020.
//

#include <fstream>
#include <sstream>
#include <cmath>
#include <iostream>
#include "../../node/Node.h"
#include "../../utils/CORRAUtils.h"


int main() {
    std::map<int, Node*> corra2NodeList;

    std::map<int, std::vector<std::pair<int, int> > > localTableMap;
    std::map<int, std::vector<std::pair<int, int> > > blockTableMap;
    std::map<std::pair<int, int>, std::vector<int> > allPath;       // < <source, dest>, [vector of path] >

    std::fstream localTableFile;
    std::fstream blockTableFile;
    std::string lineString;

    localTableFile.open("./../experiment/corra_v2/local_32x32r4", std::ios::in);
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
        localTableMap[sourceNodeID].push_back(std::pair<int, int> (destNodeID, nextNodeID));
        lineVector.clear();
    }
    localTableFile.close();

    blockTableFile.open("./../experiment/corra_v2/block_32x32r4", std::ios::in);
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
        blockTableMap[sourceNodeID].push_back(std::pair<int, int>(desBlockID, nextNodeID));
        lineVector.clear();
    }
    blockTableFile.close();

    int xTopoSize = 32;
    int yTopoSize = 32;
    int topoSize = xTopoSize * yTopoSize;
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
        auto *node = new Node(i);
        corra2NodeList.insert(std::pair<int, Node*>(i, node));
    }

    // update local routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceRecord : localTableMap) {
        for (std::pair<int, int> record : sourceRecord.second) {
            int sourceNodeID = sourceRecord.first;
            int destNodeID = record.first;
            int nextNodeID = record.second;
            corra2NodeList[sourceNodeID]->updateLocalRT(destNodeID, nextNodeID);
        }
    }

    // update block routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceRecord : blockTableMap) {
        for (std::pair<int, int> record : sourceRecord.second) {
            int sourceNodeID = sourceRecord.first;
            int destBlockID = record.first;
            int nextNodeID = record.second;
            corra2NodeList[sourceNodeID]->updateBlockRT(destBlockID, nextNodeID);
        }
    }

    // routing with all pair
    for (int sourceNodeID = 0; sourceNodeID < topoSize - 1; ++sourceNodeID) {
        for (int destNodeID = sourceNodeID + 1; destNodeID < topoSize; ++destNodeID) {
            std::vector<int> path;
            path.push_back(sourceNodeID);
            // routing on here
            std::map<int, std::pair<int, double> > sourceLocalRT = corra2NodeList[sourceNodeID]->getLocalRT();
            std::map<int, int> sourceBlockRT = corra2NodeList[sourceNodeID]->getBlockRT();
            if (sourceLocalRT.find(destNodeID) != sourceLocalRT.end()) {        // routing in local
                int nextNodeID = sourceLocalRT[destNodeID].first;
                while (nextNodeID != destNodeID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra2NodeList[nextNodeID]->getLocalRT()[destNodeID].first;
                }
            } else {        // routing beyond local
                int destBlockID = CORRAUtils::getNodeBlock(destNodeID, xBlockSize, yBlockSize, xTopoSize);
                int nextNodeID = sourceBlockRT[destBlockID];
                int nextBlockID = CORRAUtils::getNodeBlock(nextNodeID, xBlockSize, yBlockSize, xTopoSize);
                while (nextBlockID != destBlockID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra2NodeList[nextNodeID]->getBlockRT()[nextNodeID];
                    nextBlockID = CORRAUtils::getNodeBlock(nextBlockID, xBlockSize, yBlockSize, xTopoSize);
                }
                while (nextNodeID != destNodeID) {
                    path.push_back(nextNodeID);
                    nextNodeID = corra2NodeList[nextNodeID]->getLocalRT()[destNodeID].first;
                }
            }
            // add the last node
            path.push_back(destNodeID);
            // add path to all-pair path
            std::pair<int, int> source_dest (sourceNodeID, destNodeID);
            allPath.insert(std::pair<std::pair<int, int>, std::vector<int> >(source_dest, path));
        }
    }
    // counting diameter and average-path-length
    int diameter = 0;
    int totalPathLength = 0;
    for (std::pair<std::pair<int, int>, std::vector<int> > path : allPath) {
        if (path.second.size() > diameter) diameter = path.second.size();
        totalPathLength += path.second.size();
    }
    auto averagePathLength = (double) (totalPathLength / allPath.size());
    std::cout << "Diameter: " << diameter << std::endl;
    std::cout << "Average Path Length: " << averagePathLength << std::endl;
}