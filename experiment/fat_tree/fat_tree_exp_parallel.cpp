//
// Created by cuongbv on 28/03/2020.
//

#include <map>
#include <mutex>
#include <fstream>
#include <sstream>
#include <thread>
#include <future>
#include <iostream>
#include "../../node/FatTreeNode.h"
#include "../../utils/string_utils.hpp"
#include "../../utils/FatTreeUtils.h"

std::map<int, FatTreeNode*> fatTreeNodeList;
std::mutex mutex;

void createNodeList(int startNodeID, int endNodeID, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        mutex.lock();
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
        mutex.unlock();
    }
}

void updateTCAMRoutingTable(int startNodeID, int endNodeID, std::map<int, std::vector<std::pair<std::vector<int>, int> > > routingTableMap) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        std::vector<std::pair<std::vector<int>, int> > sourceRecord = routingTableMap[i];
        for (std::pair<std::vector<int>, int> record : sourceRecord) {
            int sourceNodeID = i;
            std::vector<int> destMaskIPAddress = record.first;
            int nextNodeID = record.second;
            mutex.lock();
            fatTreeNodeList[sourceNodeID]->updateRoutingTable(destMaskIPAddress, nextNodeID);
            mutex.unlock();
        }
    }
}

std::map<std::pair<int, int>, std::vector<int> > routingAllPairs(std::vector<std::pair<int, int> > allPairs_sourceN_destN, int numCoreSwitch, int numPods, int numSwitchPerPod) {
    std::map<std::pair<int, int>, std::vector<int> > allPaths;
    for (std::pair<int, int> sourceN_destN : allPairs_sourceN_destN) {
        int sourceNodeID = sourceN_destN.first;
        int destNodeID =sourceN_destN.second;
        std::vector<int> path;
        path.push_back(sourceNodeID);
        // true routing
        int *sourceAddress = FatTreeUtils::nodeIDToAddress(sourceNodeID, numCoreSwitch, numPods, numSwitchPerPod);
        int *destAddress = FatTreeUtils::nodeIDToAddress(destNodeID, numCoreSwitch, numPods, numSwitchPerPod);
        // find the Edge node - Gateway - of sourceNodeID
        int edgeSourceAddress[4] = {sourceAddress[0], sourceAddress[1], sourceAddress[2], 1};
        int edgeSourceNodeID = FatTreeUtils::addressToNodeID(edgeSourceAddress, numCoreSwitch, numPods, numSwitchPerPod);
        path.push_back(edgeSourceNodeID);
        // find Agg node up from Edge Gateway
        std::vector<int> suffixEdgetoAgg;
        suffixEdgetoAgg.push_back(10);
        suffixEdgetoAgg.push_back(-1);
        suffixEdgetoAgg.push_back(-1);
        suffixEdgetoAgg.push_back(destAddress[3] - 2 + (int)(numPods / 2));
        std::vector<std::pair<std::vector<int>, int> > edgeSourceRT = fatTreeNodeList[edgeSourceNodeID]->getRoutingTable();
        int aggSourceNodeID;
        for (const std::pair<std::vector<int>, int>& record : edgeSourceRT) {
            if (record.first == suffixEdgetoAgg) {
                aggSourceNodeID = record.second;
                break;
            }
        }
        path.push_back(aggSourceNodeID);
        // find Root (Core) up from Agg
        std::vector<int> suffixAggtoCore;
        suffixAggtoCore.push_back(10);
        suffixAggtoCore.push_back(-1);
        suffixAggtoCore.push_back(-1);
        suffixAggtoCore.push_back(destAddress[3]);
        std::vector<std::pair<std::vector<int>, int> > aggSourceRT = fatTreeNodeList[aggSourceNodeID]->getRoutingTable();
        int coreNodeID;
        for (const std::pair<std::vector<int>, int>& record : aggSourceRT) {
            if (record.first == suffixAggtoCore) {
                coreNodeID = record.second;
                break;
            }
        }
        path.push_back(coreNodeID);
        // find Agg down from Core
        std::vector<int> prefixCoretoAgg;
        prefixCoretoAgg.push_back(10);
        prefixCoretoAgg.push_back(destAddress[1]);
        prefixCoretoAgg.push_back(-1);
        prefixCoretoAgg.push_back(-1);
        std::vector<std::pair<std::vector<int>, int> > coreRT = fatTreeNodeList[coreNodeID]->getRoutingTable();
        int aggDestNodeID;
        for (const std::pair<std::vector<int>, int>& record : coreRT) {
            if (record.first == prefixCoretoAgg) {
                aggDestNodeID = record.second;
                break;
            }
        }
        path.push_back(aggDestNodeID);
        // find Edge (Gateway of Destination) down from Agg
        std::vector<int> prefixAggtoEdge;
        prefixAggtoEdge.push_back(10);
        prefixAggtoEdge.push_back(destAddress[1]);
        prefixAggtoEdge.push_back(destAddress[2]);
        prefixAggtoEdge.push_back(-1);
        std::vector<std::pair<std::vector<int>, int> > aggDestRT = fatTreeNodeList[aggDestNodeID]->getRoutingTable();
        int edgeDestNodeID;
        for (const std::pair<std::vector<int>, int>& record : aggDestRT) {
            if (record.first == prefixAggtoEdge) {
                edgeDestNodeID = record.second;
                break;
            }
        }
        path.push_back(edgeDestNodeID);
        // add the last node
        path.push_back(destNodeID);
        // add path to all pair paths
        std::pair<int, int> source_dest (sourceNodeID, destNodeID);
        allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> > (source_dest, path));
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
    std::map<int, std::vector<std::pair<std::vector<int>, int> > > routingTableMap;
    std::fstream routingTableFile;
    std::string lineString;

    routingTableFile.open("./../experiment/fat_tree/k_100", std::ios::in);
    while (!routingTableFile.eof()) {
        std::getline(routingTableFile, lineString);
        std::istringstream istringstream(lineString);       // "10 10.24.240.-1 25"
        int sourceNodeID, nextNodeID;
        std::string destMaskIPAddress;
        istringstream >> sourceNodeID;      // sourceNodeID = 10
        istringstream >> destMaskIPAddress;     // destMaskIPAddress = "10.24.240.-1"
        istringstream >> nextNodeID;        // nextNodeID = 25

        std::vector<std::string> destMarkIPAddrStrVct;
        split(destMaskIPAddress, destMarkIPAddrStrVct, '.');
        std::vector<int> ipAddressMask;
        ipAddressMask.reserve(destMarkIPAddrStrVct.size());
        for (const std::string& str : destMarkIPAddrStrVct) {
            ipAddressMask.push_back(std::stoi(str));
        }

        std::pair<std::vector<int>, int> ipMask_nextN (ipAddressMask, nextNodeID);
        routingTableMap[sourceNodeID].push_back(ipMask_nextN);
    }
    routingTableFile.close();

    int paramK = 100;
    int numCoreSwitch = paramK * paramK;
    int numPods = paramK;
    int numSwitchPerPod = paramK;
    int totalServers = paramK * paramK * paramK / 4;
    int totalVertices = numCoreSwitch + numPods * numSwitchPerPod + totalServers;

    int numSubThread = 4;
    int totalSwitch = numCoreSwitch + numPods * numSwitchPerPod;
    int subSetSwitch = (int)(totalSwitch / numSubThread);
    if (totalSwitch - subSetSwitch * numSubThread != 0) subSetSwitch++;
    int partitionSwitch[numSubThread + 1];
    int startSwitchID = 0;
    partitionSwitch[0] = 0;
    for (int part = 1; part < (int)(sizeof(partitionSwitch)/ sizeof(*partitionSwitch)); ++part) {
        partitionSwitch[part] = startSwitchID + subSetSwitch;
        startSwitchID = partitionSwitch[part];
    }

    std::vector<std::thread> threads;
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(createNodeList, partitionSwitch[i], partitionSwitch[i + 1], numCoreSwitch, numPods, numSwitchPerPod)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    threads.clear();
    threads.reserve(numSubThread);
    for (int i = 0; i < numSubThread; ++i) {
        threads.push_back(std::move(std::thread(updateTCAMRoutingTable, partitionSwitch[i], partitionSwitch[i + 1], routingTableMap)));
    }
    for (std::thread &thread : threads) {
        thread.join();
    }

    std::vector<std::pair<int, int> > allPairs_sourceN_destN;
    for (int source = totalSwitch; source < totalVertices - 1; ++source) {
        for (int dest = source + 1; dest < totalVertices; ++dest) {
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
        std::future<std::map<std::pair<int, int>, std::vector<int> > > routingAllPairsThread = std::async(std::launch::async, routingAllPairs, allPairs[i], numCoreSwitch, numPods, numSwitchPerPod);
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