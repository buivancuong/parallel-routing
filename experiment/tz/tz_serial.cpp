//
// Created by cuongbv on 29/02/2020.
//

#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include <chrono>
#include <fstream>
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/TZNode.h"
#include "../../utils/TZUtils.h"


int main() {
    int xTopoSize = 32;
    int yTopoSize = 64;
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

    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));

    auto begin = std::chrono::system_clock::now();
    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    auto doneTopo = std::chrono::system_clock::now();

    std::map<int, TZNode*> tzNodeList;
    std::map<int, TZNode*> landmarkSet;
    std::list<int> potentialLandmarkW;

    int topoSize = smallWorld2DGrid->getNumVertices();
    for (int i = 0; i < topoSize; ++i) {
        auto *tzNode = new TZNode(i);
        // std::cout << "create node " << tzNode->getNodeID() << std::endl;
        tzNode->setClosetLandmark(-1);
        tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
        potentialLandmarkW.push_back(i);
    }

    for (std::pair<int, TZNode*> tzNode : tzNodeList) {
        tzNode.second->createTraceMap(smallWorld2DGrid);
    }

    while (!potentialLandmarkW.empty()) {
        std::list<int> newLandmarks = TZUtils::sampleTZ(potentialLandmarkW, thresholeS);
        for (int landmark : newLandmarks) {
            landmarkSet.insert(std::pair<int, TZNode*>(landmark, tzNodeList[landmark]));
        }

        for (std::pair<int, TZNode*> tzNode : tzNodeList) {
            tzNode.second->extractClosetLandmark(landmarkSet);
        }

        for (std::pair<int, TZNode*> tzNode : tzNodeList) {
            tzNode.second->extractCluster(tzNodeList);
        }

        potentialLandmarkW.clear();
        for (std::pair<int, TZNode*> tzNode : tzNodeList) {
            if (tzNode.second->getCluster().size() > 4 * tzNodeList.size() / thresholeS) {
                // std::cout << "Potential landmark " << tzNode.first << std::endl;
                potentialLandmarkW.push_back(tzNode.first);
            } else {
                // std::cout << "deo can xac dinh them landmark" << std:: endl;
            }
        }
    }

    auto doneAlgo = std::chrono::system_clock::now();

    std::fstream routingTableFile;
    std::string routingTableFileName ("./../experiment/tz/");

    std::chrono::duration<double> elapsed_seconds1 = doneTopo - begin;
    std::chrono::duration<double> elapsed_seconds2 = doneAlgo - doneTopo;
    std::cout << "Create topo on " << elapsed_seconds1.count() << std::endl;
    std::cout << "Done Algorithm on " << elapsed_seconds2.count() << std::endl;

    return 0;
}