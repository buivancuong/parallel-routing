//
// Created by cuongbv on 23/10/2019.
//


#include <cstdlib>
#include "VerticeUtils.h"

int VerticeUtils::getVerticeHorizontal(int verticeID, int xTopoSize) {
    return verticeID % xTopoSize;
}

int VerticeUtils::getVerticeVertical(int verticeID, int yTopoSize) {
    return (int) (verticeID / yTopoSize);
}

int VerticeUtils::getVerticeBlock(int verticeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int xBlock = (int) (VerticeUtils::getVerticeHorizontal(verticeID, xTopoSize) / xBlockSize);
    int yBlock = (int) (VerticeUtils::getVerticeVertical(verticeID, yTopoSize) / yBlockSize);
    return yBlock * (int)(xTopoSize / xBlockSize) + xBlock;
}

int VerticeUtils::getCenterVertice(int blockId, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int blockHorizontal = VerticeUtils::getBlockHorizontal(blockId, xBlockSize, xTopoSize);
    int blockVertical = VerticeUtils::getBlockVertical(blockId, yBlockSize, yTopoSize);
    int minHorizontal = xBlockSize * blockHorizontal + (int)(xBlockSize / 2) - 1;   // -1 because of counting from 0
    int minVertical = yBlockSize * blockVertical + (int)(yBlockSize / 2) - 1;
    return minVertical * xTopoSize + minHorizontal;
}

int VerticeUtils::getBlockHorizontal(int blockId, int xBlockSize, int xTopoSize) {
    int numXBlock = (int)(xTopoSize / xBlockSize);
    return blockId % numXBlock;
}

int VerticeUtils::getBlockVertical(int blockID, int yBlockSize, int yTopoSize) {
    int numYBlock = (int)(yTopoSize / yBlockSize);
    return (int)(blockID / numYBlock);
}

int VerticeUtils::getMaxHopinBlock(int verticeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    int itsBlockID = VerticeUtils::getVerticeBlock(verticeID, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    int blockHorizontal = VerticeUtils::getBlockHorizontal(itsBlockID, xBlockSize, xTopoSize);
    int blockVertical = VerticeUtils::getBlockVertical(itsBlockID, yBlockSize, yTopoSize);
    int minHorizontal = xBlockSize * blockHorizontal;
    int maxHorizontal = minHorizontal + xBlockSize;
    int minVertical = yBlockSize * blockVertical;
    int maxVertical = minVertical + yBlockSize;
    int maxXHop, maxYHop;
    int verticeHorizontal = VerticeUtils::getVerticeHorizontal(verticeID, xTopoSize);
    int verticeVertical = VerticeUtils::getVerticeVertical(verticeID, yTopoSize);
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

VerticeUtils::~VerticeUtils() = default;

VerticeUtils::VerticeUtils() = default;
