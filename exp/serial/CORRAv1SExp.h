//
// Created by cuongbv on 20/02/2020.
//

#ifndef PARALLEL_ROUTING_CORRAV1SEXP_H
#define PARALLEL_ROUTING_CORRAV1SEXP_H


#include <vector>

class CORRAv1SExp {
public:
    CORRAv1SExp(int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize, int deltaNeighbor , const std::vector<float>& alphas);
    ~CORRAv1SExp();
};


#endif //PARALLEL_ROUTING_CORRAV1SEXP_H
