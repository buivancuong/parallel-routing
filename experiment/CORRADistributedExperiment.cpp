//
// Created by cuongbv on 05/01/2020.
//

#include "CORRADistributedExperiment.h"

#include <cmath>

CORRADistributedExperiment::~CORRADistributedExperiment() = default;

//CORRADistributedExperiment::CORRADistributedExperiment(int xTopoSize, int yTopoSize, int deltaNeighbor, std::vector<double> alphas) {
//    this->xTopoSize = xTopoSize;
//    this->yTopoSize = yTopoSize;
//    this->deltaNeighbor = deltaNeighbor;
//    if ((int)(log2(xTopoSize)) % 2 == 0) {
//        this->xBlockSize = (int) sqrt(xTopoSize);
//    } else {
//        this->xBlockSize = (int) sqrt(xTopoSize / 2);
//    }
//    if ((int) (log2(yTopoSize)) % 2 == 0) {
//        this->yBlockSize = (int) sqrt(yTopoSize);
//    } else {
//        this->yBlockSize = (int) sqrt(yTopoSize / 2);
//    }
//    this->topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, )
//}

