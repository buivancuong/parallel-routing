//
// Created by cuongbv on 29/02/2020.
//

#include <vector>
#include <list>
#include <cmath>
#include <iostream>
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/TZNode.h"
#include "../../utils/TZUtils.h"


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

    auto *smallWorld2DGrid = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);

    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, TZNode*> tzNodeList;
    std::map<int, TZNode*> landmarkSet;
    std::list<int> potentialLandmarkW;

    int topoSize = topo->getNumVertices();
    for (int i = 0; i < topoSize; ++i) {
        auto *tzNode = new TZNode(i);
        std::cout << "create node " << tzNode->getNodeID() << std::endl;
        tzNode->setClosetLandmark(-1);
        tzNodeList.insert(std::pair<int, TZNode*>(i, tzNode));
        potentialLandmarkW.push_back(i);
    }

    for (std::pair<int, TZNode*> tzNode : tzNodeList) {
        tzNode.second->createTraceMap(topo);
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
                std::cout << "Potential landmark " << tzNode.first << std::endl;
                potentialLandmarkW.push_back(tzNode.first);
            } else {
                std::cout << "deo can xac dinh them landmark" << std:: endl;
            }
        }
    }

    return 0;
}