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

int CORRAUtils::getBlockVertical(int blockID, int xBlockSize, int xTopoSize) {
    int numXBlock = (int)(xTopoSize / xBlockSize);
    return (int)(blockID / numXBlock);
}

int CORRAUtils::getMaxHopinBlock(int nodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    int itsBlockID = CORRAUtils::getNodeBlock(nodeID, xBlockSize, yBlockSize, xTopoSize);
    int blockHorizontal = CORRAUtils::getBlockHorizontal(itsBlockID, xBlockSize, xTopoSize);
    int blockVertical = CORRAUtils::getBlockVertical(itsBlockID, xBlockSize, xTopoSize);
    int minHorizontal = xBlockSize * blockHorizontal;
    int maxHorizontal = minHorizontal + xBlockSize - 1;
    int minVertical = yBlockSize * blockVertical;
    int maxVertical = minVertical + yBlockSize - 1;
    int maxXHop, maxYHop;
    int vertexHorizontal = CORRAUtils::getNodeHorizontal(nodeID, xTopoSize);
    int vertexVertical = CORRAUtils::getNodeVertical(nodeID, xTopoSize);
    if (abs(vertexHorizontal - maxHorizontal) > abs(vertexHorizontal - minHorizontal)) {
        maxXHop = abs(vertexHorizontal - maxHorizontal);
    } else {
        maxXHop = abs(vertexHorizontal - minHorizontal);
    }
    if (abs(vertexVertical - maxVertical) > abs(vertexVertical - minVertical)) {
        maxYHop = abs(vertexVertical - maxVertical);
    } else {
        maxYHop = abs(vertexVertical - minVertical);
    }
    return (maxXHop + maxYHop);
}

int CORRAUtils::getGridHop(int sourceID, int destID, int xTopoSize) {
    int sourceRow = (int) (sourceID / xTopoSize);
    int destRow = (int)(destID / xTopoSize);
    int sourceCol = sourceID % xTopoSize;
    int destCol = destID % xTopoSize;
    return abs(sourceRow - destRow) + abs(sourceCol - destCol);
}

int CORRAUtils::getTotalBlocks(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    return (int)(xTopoSize / xBlockSize) * (int)(yTopoSize / yBlockSize);
}

CORRAUtils::~CORRAUtils() = default;

CORRAUtils::CORRAUtils() = default;