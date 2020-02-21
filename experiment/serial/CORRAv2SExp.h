//
// Created by cuongbv on 21/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRAV2SEXP_H
#define PARALLEL_ROUTING_CORRAV2SEXP_H


#include <vector>

class CORRAv2SExp {
public:
    CORRAv2SExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor , const std::vector<float>& alphas);
    ~CORRAv2SExp();
};


#endif //PARALLEL_ROUTING_CORRAV2SEXP_H
