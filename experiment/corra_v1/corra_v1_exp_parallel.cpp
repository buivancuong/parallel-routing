//
// Created by cuongbv on 19/03/2020.
//

#include <mutex>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <cmath>
#include <iostream>
#include "../../node/CORRANode.h"
#include "../../utils/CORRAUtils.h"

std::map<int, CORRANode*> corra1NodeList;
std::mutex mutex;

std::vector<std::pair<int, std::pair<int, int> > > loadLocalRTFile(const std::string& localPathString) {
    std::vector<std::pair<int, std::pair<int, int> > > localTableVector;
    std::fstream localRTFile;
    std::string lineString;
    localRTFile.open(localPathString, std::ios::in);
    while (localRTFile.eof()) {
        std::getline(localRTFile, lineString);
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
    return localTableVector;
}

std::vector<std::pair<int, std::pair<int, int> > > loadBlockRTFile(const std::string& blockPathString) {
    std::vector<std::pair<int, std::pair<int, int> > > blockTableVector;
    std::fstream blockRTFile;
    std::string lineString;
    blockRTFile.open(blockPathString, std::ios::in);
    while (!blockRTFile.eof()) {
        std::getline(blockRTFile, lineString);
        std::vector<std::string> liveVector;
        std::istringstream istringstream(lineString);
        for (std::string string; istringstream >> string;) {
            liveVector.push_back(string);
        }
        int sourceNodeID = std::stoi(liveVector[0]);
        int destBlockID = std::stoi(liveVector[1]);
        int nextNodeID = std::stoi(liveVector[2]);
        std::pair<int, int> destB_nextN(destBlockID, nextNodeID);
        std::pair<int, std::pair<int, int> > sourceN_destB(sourceNodeID, destB_nextN);
        blockTableVector.push_back(sourceN_destB);
        liveVector.clear();
    }
    return blockTableVector;
}

void createNodeList(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *corraNode = new CORRANode(i);
        mutex.lock();
        corra1NodeList.insert(std::pair<int, CORRANode*>(i, corraNode));
        mutex.unlock();
    }
}

void updateLocalRT(int startNodeID, int endNodeID, std::vector<std::pair<int, std::pair<int, int> > > localTableVector) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        int sourceNodeID = i;
        int destNodeID = localTableVector[i].second.first;
        int nextNodeID = localTableVector[i].second.second;
        mutex.lock();
        corra1NodeList[sourceNodeID]->updateLocalRT(destNodeID, nextNodeID);
        mutex.unlock();
    }
}

void updateBlockRT(int startNodeID, int endNodeID, std::vector<std::pair<int, std::pair<int, int> > > blockTableVector) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        int sourceNodeID = i;
        int destBlockID = blockTableVector[i].second.first;
        int nextNodeID = blockTableVector[i].second.second;
        mutex.lock();
        corra1NodeList[sourceNodeID]->updateBlockRT(destBlockID, nextNodeID);
        mutex.unlock();
    }
}

std::map<std::pair<int, int>, std::vector<int> > routingAllPairs(const std::vector<std::pair<int, int> >& allPairs_sourceN_destN, int xBlockSize, int yBlockSize, int xTopoSize) {
    std::map<std::pair<int, int>, std::vector<int> > allPaths;
    for (std::pair<int, int> sourceN_destN : allPairs_sourceN_destN) {
        int sourceNodeID = sourceN_destN.first;
        int destNodeID = sourceN_destN.second;
        std::vector<int> path;
        path.push_back(sourceNodeID);
        // routing on here
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
        mutex.lock();
        allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> >(source_dest, path));
        mutex.unlock();
    }
    return allPaths;
}

std::pair<int, double> countingParams(const std::map<std::pair<int, int>, std::vector<int> >& allPaths) {
    std::pair<int, double> diameter_averagePathLength;
    int diameter = 0;
    int totalPathLength = 0;
    for (std::pair<std::pair<int, int>, std::vector<int> > path : allPaths) {
        if (path.second.size() > diameter) diameter = path.second.size();
        totalPathLength += path.second.max_size();
    }
    diameter_averagePathLength.first = diameter;
    diameter_averagePathLength.second = (double) (totalPathLength / allPaths.size());
    return diameter_averagePathLength;
}

int main() {

    std::future<std::vector<std::pair<int, std::pair<int, int> > > > loadLocalTableFile = std::async(std::launch::async, loadLocalRTFile, "./../experiment/corra_v1/local_32x32r4");
    std::vector<std::pair<int, std::pair<int, int> > > localTableVector = loadLocalTableFile.get();

    std::future<std::vector<std::pair<int, std::pair<int, int> > > > loadBlockTableFile = std::async(std::launch::async, loadBlockRTFile, "./../experiment/corra_v1/block_32x32r4");
    std::vector<std::pair<int, std::pair<int, int> > > blockTableVector = loadBlockTableFile.get();

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

    int numSubThread = 4;
    int subSetNode = (int)(xTopoSize * yTopoSize / numSubThread);
    if (xTopoSize * yTopoSize - numSubThread * subSetNode != 0) subSetNode++;
    int partitionNode[numSubThread + 1];
    int startNodeID = 0;
    partitionNode[0] = 0;
    for (int part = 1; part < (int)(sizeof(partitionNode) / sizeof(*partitionNode)); ++part) {
        partitionNode[part] = startNodeID + subSetNode;
        startNodeID = partitionNode[part];
    }

    std::vector<std::thread> threads;
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(createNodeList, partitionNode[i], partitionNode[i + 1])));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(updateLocalRT, partitionNode[i], partitionNode[i + 1], localTableVector)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(updateBlockRT, partitionNode[i], partitionNode[i + 1], blockTableVector)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    std::vector<std::pair<int, int> > allPairs_sourceN_destN;
    for (int source = 0; source < topoSize - 1; ++source) {
        for (int dest = source + 1; dest < topoSize; ++dest) {
            allPairs_sourceN_destN.emplace_back(source, dest);
        }
    }
    int subSetPair = (int)(allPairs_sourceN_destN.size() / numSubThread);
    if (allPairs_sourceN_destN.size() - numSubThread * subSetPair != 0) subSetPair++;
    int partitionPair[numSubThread + 1];
    int startPair = 0;
    partitionPair[0] = 0;
    for (int part = 1; part < (int)(sizeof(partitionPair) / sizeof(*partitionPair)); ++part) {
        partitionPair[part] = startPair + subSetPair;
        startPair = partitionPair[part];
    }

    std::vector<std::vector<std::pair<int, int> > > allPairs;
    allPairs.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::vector<std::pair<int, int> > paths;
        for (int j = partitionPair[i]; j < partitionPair[i + 1]; ++j) {
            paths.push_back(allPairs_sourceN_destN[j]);
        }
        allPairs.push_back(paths);
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(routingAllPairs, allPairs[i], xBlockSize, yBlockSize, xTopoSize)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    std::vector<std::map<std::pair<int, int>, std::vector<int> > > allPaths;
    allPaths.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::future<std::map<std::pair<int, int>, std::vector<int> > > routingAllPairsThread = std::async(std::launch::async, routingAllPairs, allPairs[i], xBlockSize, yBlockSize, xTopoSize);
        allPaths.push_back(routingAllPairsThread.get());
    }

    std::vector<std::pair<int, double> > paramsVector;
    paramsVector.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::future<std::pair<int, double> > countingParamsThread = std::async(std::launch::async, countingParams, allPaths[i]);
        paramsVector.push_back(countingParamsThread.get());
    }

    int diameter = 0;
    double totalPathLength = 0;
    for (std::pair<int, double> param : paramsVector) {
        if (param.first > diameter) diameter = param.first;
        totalPathLength += param.second;
    }
    double averagePathLength = totalPathLength / numSubThread;
    std::cout << "Diameter: " << diameter << std::endl;
    std::cout << "Average Path Length: " << averagePathLength << std::endl;
}