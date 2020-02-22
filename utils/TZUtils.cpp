//
// Created by cuongbv on 22/02/2020.
//

#include <cstdlib>
#include "TZUtils.h"

TZUtils::TZUtils() = default;

TZUtils::~TZUtils() = default;

std::list<int> TZUtils::sampleTZ(std::list<int> potentialLandmarkW, float thresholeS) {
    if (potentialLandmarkW.empty()) return potentialLandmarkW;
    if (potentialLandmarkW.size() < thresholeS) return potentialLandmarkW;

    float probability = thresholeS / potentialLandmarkW.size();
    std::list<int> newLandmarks;
    for (int nodeID : potentialLandmarkW) {
        float random = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        if (random < probability) {
            newLandmarks.push_back(nodeID);
        }
    }
    return newLandmarks;
}
