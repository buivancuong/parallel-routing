//
// Created by cuongbv on 20/12/2019.
//

#include <cstdlib>
#include "CORRAUtils.h"

int CORRAUtils::getNodeHorizontal(int nodeID, int xTopoSize) {
    return nodeID % xTopoSize;
}

int CORRAUtils::getNodeVertical(int nodeID, int xTopoSize) {
    return (int) (nodeID / xTopoSize);
}

int CORRAUtils::getNodeBlock(int nodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    int xBlock = (int) (CORRAUtils::getNodeHorizontal(nodeID, xTopoSize) / xBlockSize);
    int yBlock = (int) (CORRAUtils::getNodeVertical(nodeID, xTopoSize) / yBlockSize);
    return yBlock * (int)(xTopoSize / xBlockSize) + xBlock;
}

int CORRAUtils::getCenterVertex(int blockId, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int blockHorizontal = CORRAUtils::getBlockHorizontal(blockId, xBlockSize, xTopoSize);
    int blockVertical = CORRAUtils::getBlockVertical(blockId, yBlockSize, yTopoSize);
    int minHorizontal = xBlockSize * blockHorizontal + (int)(xBlockSize / 2) - 1;   // -1 because of counting from 0
    int minVertical = yBlockSize * blockVertical + (int)(yBlockSize / 2) - 1;
    return minVertical * xTopoSize + minHorizontal;
}

int CORRAUtils::getBlockHorizontal(int blockId, int xBlockSize, int xTopoSize) {
    int numXBlock = (int)(xTopoSize / xBlockSize);
    return blockId % numXBlock;
}

int CORRAUtils::getBlockVertical(int blockID, int yBlockSize, int yTopoSize) {
    int numYBlock = (int)(yTopoSize / yBlockSize);
    return (int)(blockID / numYBlock);
}

int CORRAUtils::getMaxHopinBlock(int nodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int itsBlockID = CORRAUtils::getNodeBlock(nodeID, xBlockSize, yBlockSize, xTopoSize);
    int blockHorizontal = CORRAUtils::getBlockHorizontal(itsBlockID, xBlockSize, xTopoSize);
    int blockVertical = CORRAUtils::getBlockVertical(itsBlockID, yBlockSize, yTopoSize);
    int minHorizontal = xBlockSize * blockHorizontal;
    int maxHorizontal = minHorizontal + xBlockSize;
    int minVertical = yBlockSize * blockVertical;
    int maxVertical = minVertical + yBlockSize;
    int maxXHop, maxYHop;
    int verticeHorizontal = CORRAUtils::getNodeHorizontal(nodeID, xTopoSize);
    int verticeVertical = CORRAUtils::getNodeVertical(nodeID, xTopoSize);
    if (abs(verticeHorizontal - maxHorizontal) > abs(verticeHorizontal - minHorizontal)) {
        maxXHop = maxHorizontal;
    } else {
        maxXHop = minHorizontal;
    }
    if (abs(verticeVertical - maxVertical) > abs(verticeVertical - minVertical)) {
        maxYHop = maxHorizontal;
    } else {
        maxYHop = minVertical;
    }
    return (maxXHop + maxYHop);
}

int CORRAUtils::getGridHop(int sourceID, int destID, int xTopoSize) {
    int sourceRow = sourceID / xTopoSize;
    int destRow = destID / xTopoSize;
    int sourceCol = sourceID % xTopoSize;
    int destCol = destID % xTopoSize;
    return abs(sourceRow - destRow) + abs(sourceCol - destCol);
}

CORRAUtils::~CORRAUtils() = default;

CORRAUtils::CORRAUtils() = default;