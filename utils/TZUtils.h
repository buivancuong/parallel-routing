//
// Created by cuongbv on 22/02/2020.
//

#ifndef PARALLEL_ROUTING_TZUTILS_H
#define PARALLEL_ROUTING_TZUTILS_H


#include <list>

class TZUtils {
public:
    TZUtils();
    ~TZUtils();

    static std::list<int> sampleTZ(std::list<int> potentialLandmarkW, float thresholeS);

};


#endif //PARALLEL_ROUTING_TZUTILS_H
