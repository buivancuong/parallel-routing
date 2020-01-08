#include <iostream>
#include <ctime>
#include <cmath>
#include "graph/smallworld/SmallWorld2DGrid.h"
#include "node/Node.h"

int main() {
    int xTopoSize = 64;
    int yTopoSize = 64;
    int deltaNeighbor = 8;
    std::vector<float> alphas = {1.6, 2};

    int xBlockSize, yBlockSize;
    if ((int)(log2(xTopoSize)) % 2 == 0) {
        xBlockSize = (int) sqrt(xTopoSize);
    } else {
        xBlockSize = (int) sqrt(xTopoSize / 2);
    }
    if ((int) (log2(yTopoSize)) % 2 == 0) {
        yBlockSize = (int) sqrt(yTopoSize);
    } else {
        yBlockSize = (int) sqrt(yTopoSize / 2);
    }

    SmallWorld2DGrid topo(yTopoSize, xTopoSize, alphas);

    std::map<int, Node> nodeList;
    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        Node node(i);
        nodeList.insert(std::pair<int, Node>(i, node));
    }

    for (std::pair<int, std::map<int, float> > sourceNode : topo.getAdjList()) {
        std::cout << "Node " << sourceNode.first << std::endl;
        for (std::pair<int, float> neighbor : sourceNode.second) {
            if (neighbor.second == 1) {
                std::cout << "add near " << neighbor.first << std::endl;
                nodeList[sourceNode.first].addNearNeighbors(nodeList[neighbor.first]);
            } else {
                std::cout << "add far " << neighbor.first << std::endl;
                nodeList[sourceNode.first].addFarNeighbors(nodeList[neighbor.first]);
            }
        }
        std::cout << std::endl;
    }


    return  0;

}