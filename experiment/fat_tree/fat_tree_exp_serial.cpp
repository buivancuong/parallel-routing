//
// Created by cuongbv on 27/03/2020.
//

#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../../node/FatTreeNode.h"
#include "../../utils/string_utils.hpp"
#include "../../utils/FatTreeUtils.h"

int main() {
    std::map<int, FatTreeNode*> fatTreeNodeList;

    std::map<int, std::vector<std::pair<std::vector<int>, int> > > routingTableMap;      // <sourceN, [<ipMask, destN>] >
    std::map<std::pair<int, int>, std::vector<int> > allPaths;

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

    // create Fat Tree node list
    for (int i = 0; i < totalVertices; ++i) {
        auto *fatTreeNode = new FatTreeNode(i, numCoreSwitch, numPods, numSwitchPerPod);
        fatTreeNodeList.insert(std::pair<int, FatTreeNode*>(i, fatTreeNode));
    }

    // update TCAM routing table
    for (std::pair<int, std::vector<std::pair<std::vector<int>, int> > > sourceRecord : routingTableMap) {
        for (const std::pair<std::vector<int>, int>& record : sourceRecord.second) {
            int sourceNodeID = sourceRecord.first;
            std::vector<int> destMaskIPAddress = record.first;
            int nextNodeID = record.second;
            fatTreeNodeList[sourceNodeID]->updateRoutingTable(destMaskIPAddress, nextNodeID);
        }
    }
    // TODO: check Dijkstra shortest path trace map at sourceNodeID for general case

    // routing with all pairs of totalServers
    int startServerID = totalVertices - totalServers;
    int endServerID = totalVertices;
    for (int sourceNodeID = startServerID; sourceNodeID < endServerID - 1; ++sourceNodeID) {
        for (int destNodeID = startServerID + 1; destNodeID < endServerID; ++destNodeID) {
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
            std::pair<int, int> sourceN_destN (sourceNodeID, destNodeID);
            allPaths.insert(std::pair<std::pair<int, int>, std::vector<int> > (sourceN_destN, path));
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

    return 0;
}