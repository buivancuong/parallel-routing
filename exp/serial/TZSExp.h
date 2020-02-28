//
// Created by cuongbv on 21/02/2020.
//

#ifndef PARALLEL_ROUTING_TZSEXP_H
#define PARALLEL_ROUTING_TZSEXP_H


#include <vector>

class TZSExp {
public:
    TZSExp(int xTopoSize, int yTopoSize, const std::vector<float>& alphas);
    ~TZSExp();

};


#endif //PARALLEL_ROUTING_TZSEXP_H
