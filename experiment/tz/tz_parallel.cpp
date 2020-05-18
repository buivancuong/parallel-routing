//
// Created by cuongbv on 29/02/2020.
//

#include <cmath>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
<<<<<<< HEAD
=======
#include <iostream>
#include <fstream>
>>>>>>> graphlib
#include "../../node/TZNode.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/TZUtils.h"

std::map<int, TZNode*> tzNodeList;
std::mutex mutex;
<<<<<<< HEAD
//std::map<int, TZNode*> landmarkSet;
//std::list<int> potentialLandmarkW;
=======
>>>>>>> graphlib

void createTZNodeList(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *tzNode = new TZNode(i);
        tzNode->setClosetLandmark(-1);
        mutex.lock();
        tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
        mutex.unlock();
    }
}

void createTraceMap(int startNodeID, int endNodeID, Graph *globalGraph) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        tzNodeList[i]->createTraceMap(globalGraph);
    }
}

void extractClosetLandmark(int startNodeID, int endNodeID, const std::map<int, TZNode *>& landmarks) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        tzNodeList[i]->extractClosetLandmark(landmarks);
    }
}

void extractCluster(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        tzNodeList[i]->extractCluster(tzNodeList);
    }
}

<<<<<<< HEAD
int main() {
    int xTopoSize = 32;
    int yTopoSize = 32;
    int deltaNeighbor = 3;
=======
void createClusterRT(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        tzNodeList[i]->createClusterRT();
    }
}

void createLandmarkRT(int startNodeID, int endNodeID, const std::map<int, TZNode*>& landmarkSet) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        tzNodeList[i]->createLandmarkRT(landmarkSet);
    }
}

int main() {
    int xTopoSize = 32;
    int yTopoSize = 64;
>>>>>>> graphlib
    std::vector<float> alphas = {1.6, 2};

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
<<<<<<< HEAD
    int numBlock = (xTopoSize / xBlockSize) * (yTopoSize / yBlockSize);

    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
=======

    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));


    auto begin = std::chrono::system_clock::now();
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    auto doneTopo = std::chrono::system_clock::now();


>>>>>>> graphlib
    std::map<int, TZNode*> landmarkSet;
    std::list<int> potentialLandmarkW;

    int numSubThread = 4;
    int subSet = (int)(xTopoSize * yTopoSize / numSubThread) + 1;
    int partition[numSubThread + 1];
    int start = 0;
    partition[0] = 0;
    for (int part = 1; part < (int)(sizeof(partition)/ sizeof(*partition)); ++part) {
        partition[part] = start + subSet;
        start = partition[part];
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createTZNodeList, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    for (std::pair<int, TZNode*> tzNode : tzNodeList) {
        potentialLandmarkW.push_back(tzNode.first);
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createTraceMap, partition[i], partition[i + 1], smallWorld2DGrid);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    while (!potentialLandmarkW.empty()) {
        std::list<int> newLandmarks = TZUtils::sampleTZ(potentialLandmarkW, thresholeS);
        for (int landmark : newLandmarks) {
            landmarkSet.insert(std::pair<int, TZNode*>(landmark, tzNodeList[landmark]));
        }

        threads.clear();
        for (int i = 0; i < numSubThread; ++i) {
            std::thread thread(extractClosetLandmark, partition[i], partition[i + 1], landmarkSet);
            threads.push_back(std::move(thread));
        }
        for (auto &thread : threads) {
            thread.join();
        }

        threads.clear();
        for (int i = 0; i < numSubThread; ++i) {
            std::thread thread(extractCluster, partition[i], partition[i + 1]);
            threads.push_back(std::move(thread));
        }
        for (auto &thread : threads) {
            thread.join();
        }

        potentialLandmarkW.clear();
        for (std::pair<int, TZNode*> tzNode : tzNodeList) {
            if (tzNode.second->getCluster().size() > 4 * tzNodeList.size() / thresholeS) {
                potentialLandmarkW.push_back(tzNode.first);
            }
        }
    }

<<<<<<< HEAD
=======
    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createClusterRT, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(createLandmarkRT, partition[i], partition[i + 1], landmarkSet);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    auto doneAlgo = std::chrono::system_clock::now();

    std::fstream clusterTableFile;
    std::fstream landmarkTableFile;
    std::string localTableFileName ("./../experiment/tz/cluster_" + std::to_string(xTopoSize) + "x" + std::to_string(yTopoSize));
    std::string blockTableFileName ("./../experiment/tz/landmark_" + std::to_string(xTopoSize) + "x" + std::to_string(yTopoSize));

    clusterTableFile.open(localTableFileName.c_str(), std::ios::out);
    // routing table format on cluster: <currentNodeID destNodeID nextNodeID>
    for (std::pair<int, TZNode*> tzNode : tzNodeList) {
        std::map<int, int> clusterRT = tzNode.second->getClusterRT();
        for (std::pair<int, int> destNodeID : clusterRT) {
            std::string row (std::to_string(tzNode.first) + " " + std::to_string(destNodeID.first) + " " + std::to_string(destNodeID.second) + "\n");
            clusterTableFile << row;
        }
    }
    clusterTableFile.close();

    landmarkTableFile.open(blockTableFileName.c_str(), std::ios::out);
    // routing table to the landmarks: <currentNodeID destLandmarkNodeID nextNdeID>
    for (std::pair<int, TZNode*> tzNode : tzNodeList) {
        std::map<int, int> landmarkRT = tzNode.second->getLandmarkRT();
        for (std::pair<int, int> destLandmark : landmarkRT) {
            std::string row (std::to_string(tzNode.first) + " " + std::to_string(destLandmark.first) + " " + std::to_string(destLandmark.second) + "\n");
            landmarkTableFile << row;
        }
    }
    landmarkTableFile.close();

    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

>>>>>>> graphlib
    return 0;
}