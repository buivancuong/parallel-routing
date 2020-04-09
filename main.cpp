#include <iostream>
#include <vector>
#include <cmath>
#include "utils/CORRAUtils.h"
#include "graph/Graph.h"

int main() {

    int xTopoSize = 64;
    int yTopoSize = 64;
    std::vector<float> alphas = {1.6, 2};

    int xBlockSize, yBlockSize;
    if ((int) (log2(xTopoSize)) % 2 == 0) {
        xBlockSize = (int) sqrt(xTopoSize);
    } else {
        xBlockSize = (int) sqrt(xTopoSize / 2);
    }
    if ((int) (log2(yTopoSize)) % 2 == 0) {
        yBlockSize = (int) sqrt(yTopoSize);
    } else {
        yBlockSize = (int) sqrt(yTopoSize / 2);
    }

//    auto *graph = new Graph(9);
//    graph->addEdge(0, 1, 8);
//    graph->addEdge(0, 3, 5);
//    graph->addEdge(0, 2, 2);
//    graph->addEdge(1, 3, 2);
//    graph->addEdge(1, 5, 13);
//    graph->addEdge(2, 3, 2);
//    graph->addEdge(2, 4, 5);
//    graph->addEdge(3, 5, 6);
//    graph->addEdge(3, 4, 1);
//    graph->addEdge(3, 6, 3);
//    graph->addEdge(4, 6, 1);
//    graph->addEdge(5, 6, 2);
//    graph->addEdge(5, 7, 3);
//    graph->addEdge(6, 7, 6);
//    std::map<int, std::pair<float, int> > trace = graph->Dijkstra(0, 0);
//    std::cout << "Here" << std::endl;


//    int gridHop = CORRAUtils::getGridHop(646, 579, xTopoSize);
//    std::cout << gridHop << std::endl;
    int nodeBlock = CORRAUtils::getNodeBlock(365, xBlockSize, yBlockSize, xTopoSize);
    std::cout << nodeBlock << std::endl;
//    int center = CORRAUtils::getCenterVertex(3624, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//    std::cout << center << std::endl;

    return 0;
}