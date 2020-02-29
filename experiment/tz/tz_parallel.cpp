//
// Created by cuongbv on 29/02/2020.
//

#include <cmath>
#include <vector>
#include <list>
#include <thread>
#include <mutex>
#include "../../node/TZNode.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/TZUtils.h"

std::map<int, TZNode*> tzNodeList;
std::mutex mutex;
//std::map<int, TZNode*> landmarkSet;
//std::list<int> potentialLandmarkW;

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

int main() {
    int xTopoSize = 32;
    int yTopoSize = 32;
    int deltaNeighbor = 3;
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
    int numBlock = (xTopoSize / xBlockSize) * (yTopoSize / yBlockSize);

    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
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

    return 0;
}