//
// Created by cuongbv on 22/02/2020.
//

#include <thread>
#include <cmath>
#include <list>
#include "TZPExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../utils/TZUtils.h"

void TZPExp::createTZNodeList(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        auto *tzNode = new TZNode(i);
        tzNode->setClosetLandmark(-1);
        this->tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
    }
}

void TZPExp::createTraceMap(int startNodeID, int endNodeID, Graph *globalGraph) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->tzNodeList[i]->createTraceMap(globalGraph);
    }
}

void TZPExp::extractClosetLandmark(int startNodeID, int endNodeID, const std::map<int, TZNode *>& landmarks) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->tzNodeList[i]->extractClosetLandmark(landmarks);
    }
}

void TZPExp::extractCluster(int startNodeID, int endNodeID) {
    for (int i = startNodeID; i < endNodeID; ++i) {
        this->tzNodeList[i]->extractCluster(this->tzNodeList);
    }
}

TZPExp::TZPExp(int xTopoSize, int yTopoSize, int deltaNeighbor, const std::vector<float> &alphas, int numSubThread) {
    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, TZNode*> landmarkSet;
    std::list<int> potentialLandmarkW;

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
        std::thread thread(&TZPExp::createTZNodeList, this, partition[i], partition[i + 1]);
        threads.push_back(std::move(thread));
    }
    for (auto &thread : threads) {
        thread.join();
    }

    for (std::pair<int, TZNode*> tzNode : this->tzNodeList) {
        potentialLandmarkW.push_back(tzNode.first);
    }

    threads.clear();
    for (int i = 0; i < numSubThread; ++i) {
        std::thread thread(&TZPExp::createTraceMap, this, partition[i], partition[i + 1], topo);
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
            std::thread thread(&TZPExp::extractClosetLandmark, this, partition[i], partition[i + 1], landmarkSet);
            threads.push_back(std::move(thread));
        }
        for (auto &thread : threads) {
            thread.join();
        }

        threads.clear();
        for (int i = 0; i < numSubThread; ++i) {
            std::thread thread(&TZPExp::extractCluster, this, partition[i], partition[i + 1]);
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

}

