//
// Created by cuongbv on 23/10/2019.
//

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
//    int minVerticeHorizontal =
}

int VerticeUtils::getBlockHorizontal(int blockId, int xBlockSize, int xTopoSize) {
    int numXBlock = (int)(xTopoSize / xBlockSize);
    return blockId % numXBlock;
}

int VerticeUtils::getBlockVertical(int blockID, int yBlockSize, int yTopoSize) {
    int numYBlock = (int)(yTopoSize / yBlockSize);
    return (int)(blockID / numYBlock);
}

VerticeUtils::~VerticeUtils() = default;

VerticeUtils::VerticeUtils() = default;
