//
// Created by cuongbv on 21/02/2020.
//

#include <cmath>
#include <list>
#include "TZSExp.h"
#include "../../graph/smallworld/SmallWorld2DGrid.h"
#include "../../node/TZNode.h"
#include "../../utils/TZUtils.h"

TZSExp::TZSExp(int xTopoSize, int yTopoSize, const std::vector<float> &alphas) {
    auto thresholeS = (float)(sqrt(xTopoSize * yTopoSize / log2(xTopoSize * yTopoSize)));
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
    std::map<int, TZNode*> tzNodeList;
    std::map<int, TZNode*> landmarkSet;
    std::list<int> potentialLandmarkW;

    int topoSize = topo->getNumVertices();
    for (int i = 0; i < topoSize; ++i) {
        auto *tzNode = new TZNode(i);
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
                potentialLandmarkW.push_back(tzNode.first);
            }
        }
    }

}

TZSExp::~TZSExp() = default;
