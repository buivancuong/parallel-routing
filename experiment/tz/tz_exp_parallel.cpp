//
// Created by cuongbv on 25/03/2020.
// More refer at "./tz_exp_serial.cpp"
//

#include <mutex>
#include <vector>
#include <fstream>
#include <sstream>
#include <future>
#include <iostream>
#include "../../node/TZNode.h"

std::map<int, TZNode*> tzNodeList;
std::mutex mutex;

std::map<int, std::vector<std::pair<int, int> > > loadClusterRTFile(const std::string& clusterPathString) {
    std::map<int, std::vector<std::pair<int, int> > > clusterTableMap;
    std::fstream clusterTableFile;
    std::string lineString;
    clusterTableFile.open(clusterPathString, std::ios::in);
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
    return clusterTableMap;
}

std::map<int, std::vector<std::pair<int, int> > > loadLandmarkRTFile(const std::string& landmarkPathString) {
    std::map<int, std::vector<std::pair<int, int> > > landmarkTableMap;
    std::fstream landmarkTableFile;
    std::string lineString;
    landmarkTableFile.open(landmarkPathString, std::ios::in);
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
    return landmarkTableMap;
}

std::map<int, int> loadClosetLandmarkFile(const std::string& closetLandmarkPathString) {
    std::map<int, int> closetLandmarkMap;
    std::fstream closetLandmarksFile;
    std::string lineString;
    closetLandmarksFile.open(closetLandmarkPathString, std::ios::in);
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
    return closetLandmarkMap;
}

void createNodeList(int startNodeID, int endNodeID, std::map<int, int> closetLandmarkMap) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *tzNode = new TZNode(i);
        tzNode->setClosetLandmark(closetLandmarkMap[i]);
        mutex.lock();
        tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
        mutex.unlock();
    }
}

void updateClusterRT(int startNodeID, int endNodeID, std::map<int, std::vector<std::pair<int, int> > > clusterTableMap) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        for (std::pair<int, int> innerClusterNode : clusterTableMap[i]) {
            int sourceNodeID = i;
            int destNodeID = innerClusterNode.first;
            int nextNodeID = innerClusterNode.second;
            mutex.lock();
            tzNodeList[sourceNodeID]->updateClusterRT(destNodeID, nextNodeID);
            mutex.unlock();
        }
    }
}

void updateLandmarkRT(int startNodeID, int endNodeID, std::map<int, std::vector<std::pair<int, int> > > landmarkTableMap) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        for (std::pair<int, int> landmark : landmarkTableMap[i]) {
            int sourceNodeID = i;
            int destLandmarkID = landmark.first;
            int nextNodeID = landmark.second;
            mutex.lock();
            tzNodeList[sourceNodeID]->updateLandmarkRT(destLandmarkID, nextNodeID);
            mutex.unlock();
        }
    }
}

std::map<std::pair<int, int>, std::vector<int> > routingAllPairs(const std::vector<std::pair<int, int> >& allPairs_sourceN_destN) {
    std::map<std::pair<int, int>, std::vector<int> > allPaths;

    for (std::pair<int, int> sourceN_destN : allPairs_sourceN_destN) {
        int sourceNodeID = sourceN_destN.first;
        int destNodeID = sourceN_destN.second;
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
                        nextNodeID = tzNodeList[nextNodeID]->getClusterRT()[destNodeID];
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

    std::future<std::map<int, std::vector<std::pair<int, int> > > > loadClusterTableFile = std::async(std::launch::async, loadClusterRTFile, "./../experiment/tz/cluster_32x32");
    std::map<int, std::vector<std::pair<int, int> > > clusterTableMap = loadClusterTableFile.get();

    std::future<std::map<int, std::vector<std::pair<int, int> > > > loadLandmarkTableFile = std::async(std::launch::async, loadLandmarkRTFile, "./../experiment/tz/landmark_32x32");
    std::map<int, std::vector<std::pair<int, int> > > landmarkTableMap = loadClusterTableFile.get();

    std::future<std::map<int, int> > loadClosetLandmarkListFile = std::async(std::launch::async, loadClosetLandmarkFile, "./../experiment/tz/closet_32x32");
    std::map<int, int> closetLandmarkMap = loadClosetLandmarkListFile.get();

    int xTopoSize = 32;
    int yTopoSize = 32;
    int topoSize = xTopoSize * yTopoSize;

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
        threads.push_back(std::move(std::thread(createNodeList, partitionNode[i], partitionNode[i + 1], closetLandmarkMap)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(updateClusterRT, partitionNode[i], partitionNode[i + 1], clusterTableMap)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(updateLandmarkRT, partitionNode[i], partitionNode[i + 1], landmarkTableMap)));
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

    std::vector<std::map<std::pair<int, int>, std::vector<int> > > allPaths;
    allPaths.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        std::future<std::map<std::pair<int, int>, std::vector<int> > > routingAllPairsThread = std::async(std::launch::async, routingAllPairs, allPairs[i]);
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

    return 0;
}