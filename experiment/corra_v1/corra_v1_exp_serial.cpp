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
    // TODO: checknull at end of file

    std::map<int, std::vector<std::pair<int, int> > > localTableMap;        // <sourceN, [<destN, nextN>] >
    std::map<int, std::vector<std::pair<int, int> > > blockTableMap;     // <sourceN, [<destB, nextN>] >
    std::map<std::pair<int, int>, std::vector<int> > allPaths;       // < <source, dest>, [vector of path] >

    std::fstream localTableFile;
    std::fstream blockTableFile;
    std::string lineString;

    localTableFile.open("./../experiment/corra_v1/local_64x64r6", std::ios::in);
    while (!localTableFile.eof()) {
        std::getline(localTableFile, lineString);
        if (lineString.empty()) break;
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int destNodeID = std::stoi(lineVector[1]);
        int nextNodeID = std::stoi(lineVector[2]);
        localTableMap[sourceNodeID].push_back(std::pair<int, int>(destNodeID, nextNodeID));
        lineVector.clear();
    }
    localTableFile.close();

    blockTableFile.open("./../experiment/corra_v1/block_64x64r6", std::ios::in);
    while (!blockTableFile.eof()) {
        std::getline(blockTableFile, lineString);
        if (lineString.empty()) break;
        std::vector<std::string> lineVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            lineVector.push_back(string);
        }
        int sourceNodeID = std::stoi(lineVector[0]);
        int destBlockID = std::stoi(lineVector[1]);
        int nextNodeID = std::stoi(lineVector[2]);
        blockTableMap[sourceNodeID].push_back(std::pair<int, int>(destBlockID, nextNodeID));
        lineVector.clear();
    }
    blockTableFile.close();

    int xTopoSize = 64;
    int yTopoSize = 64;
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
        auto *corraNode = new CORRANode(i);
        corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
    }

    // update local routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceRecord : localTableMap) {
        for (std::pair<int, int> record : sourceRecord.second) {
            int sourceNodeID = sourceRecord.first;
            int destNodeID = record.first;
            int nextNodeID = record.second;
            corra1NodeList[sourceNodeID]->updateLocalRT(destNodeID, nextNodeID);
        }
    }

    // update block routing table
    for (std::pair<int, std::vector<std::pair<int, int> > > sourceRecord : blockTableMap) {
        for (std::pair<int, int> record : sourceRecord.second) {
            int sourceNodeID = sourceRecord.first;
            int destBlockID = record.first;
            int nextNodeID = record.second;
            corra1NodeList[sourceNodeID]->updateBlockRT(destBlockID, nextNodeID, std::vector<int>());
        }
    }

    // routing with all pairs
    for (int sourceNodeID = 0; sourceNodeID < topoSize - 1; ++sourceNodeID) {
        for (int destNodeID = sourceNodeID + 1; destNodeID < topoSize; ++destNodeID) {
//            bool comui = false;
            std::vector<int> path;
            path.push_back(sourceNodeID);
            // true routing
            std::map<int, std::pair<int, double> > currentNodeLocalRT = corra1NodeList[sourceNodeID]->getLocalRT();
            std::map<int, std::pair<int, std::vector<int> > > currentNodeBlockRT = corra1NodeList[sourceNodeID]->getBlockRT();
            while (currentNodeLocalRT.find(destNodeID) == currentNodeLocalRT.end()) {       // neu destNodeID van chua nam trong local cua currentNodeID
//                if (path.size() > 100) {
//                    comui = true;
//                    break;
//                }
                if (path.size() > 50) {
                    std::cout << "in block" << std::endl;
                }
                int destBlockID = CORRAUtils::getNodeBlock(destNodeID, xBlockSize, yBlockSize, xTopoSize);
                int nextNodeID = currentNodeBlockRT[destBlockID].first;
                if (nextNodeID == destNodeID) {
                    path.push_back(destNodeID);
                    break;
                }
                path.push_back(nextNodeID);
                currentNodeLocalRT = corra1NodeList[nextNodeID]->getLocalRT();
                currentNodeBlockRT = corra1NodeList[nextNodeID]->getBlockRT();
            }       // con ko thi dinh tuyen trong locality
//            if (comui) {
//                std::cout << "miss " << sourceNodeID << " to " << destNodeID << std::endl;
//                continue;
//            }
            int currentNodeID = path.back();
            if (currentNodeID != destNodeID) {
                int nextNodeID = currentNodeLocalRT[destNodeID].first;
                while (nextNodeID != destNodeID) {
                    if (path.size() > 50) {
                        std::cout << "in local" << std::endl;
                    }
                    path.push_back(nextNodeID);
                    nextNodeID = corra1NodeList[nextNodeID]->getLocalRT()[destNodeID].first;
                }
                path.push_back(destNodeID);
            }
            // add the last node
            // add path to all-pair path
            std::pair<int, int> source_dest (sourceNodeID, destNodeID);
            allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> >(source_dest, path));
            std::cout << "path " << sourceNodeID << " to " << destNodeID << ": ";
            for (int i : path) {
                std::cout << i << " ";
            }
            std::cout << std::endl;
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