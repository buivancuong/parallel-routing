#include <iostream>
#include <ctime>
#include <cmath>
#include "graph/smallworld/SmallWorld2DGrid.h"
#include "node/Node.h"
#include "utils/CORRAUtils.h"

int main() {
    int xTopoSize = 64;
    int yTopoSize = 64;
    int deltaNeighbor = 3;
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
    int numBlock = (xTopoSize/xBlockSize) * (yTopoSize/yBlockSize);

    std::cout << xBlockSize << " " << yBlockSize << std::endl;

    // Create Topology
    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);

    // Create list of Nodes on Topo
    std::map<int, Node*> nodeList;
    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
        Node *node = new Node(i);
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
            std::cout << "center " << centerVertex << std::endl;
        }
        nodeList.insert(std::pair<int, Node*>(i, node));
    }

    // Add near neighbors (grid neighbor) and far neighbors (random link neighbors)
    for (std::pair<int, std::map<int, float> > sourceNode : topo->getAdjList()) {
        std::cout << "Node " << sourceNode.first << std::endl;
        for (std::pair<int, float> neighbor : sourceNode.second) {
            if (neighbor.second == 1) {     // grid neighbor
                std::cout << "add near " << neighbor.first << std::endl;
                nodeList[sourceNode.first]->addNearNeighbors(nodeList[neighbor.first]);
            } else {        // random link neighbor
                std::cout << "add far " << neighbor.first << std::endl;
                nodeList[sourceNode.first]->addFarNeighbors(nodeList[neighbor.first]);
            }
        }
        std::cout << std::endl;
    }

    std::cout << "Prepare for create the Locality" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeID " << node.first << std::endl;
        node.second->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        std::cout << std::endl;
    }
    std::cout << "*******************************" << std::endl;
    std::cout << std::endl;

    std::cout << "Create the Locality" << std::endl;
    for (std::pair<int, Node*> node: nodeList) {
        std::cout << "nodeiD " << node.first << std::endl;
        node.second->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        std::cout << std::endl;
    }
    std::cout << "Done create the locality" << std::endl;
    std::cout << std::endl;

    std::cout << "Update local routing table" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeID " << node.first << std::endl;
        node.second->createLocalRouting(xTopoSize, yTopoSize);
        std::cout << std::endl;
    }
    std::cout << "Done Update local routing table" << std::endl;
    std::cout << std::endl;

    std::cout << "Find Bridge 1" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeID " << node.first << std::endl;
        node.second->findBR1();
        std::cout << std::endl;
    }
    std::cout << "Done find bridge 1" << std::endl;
    std::cout << std::endl;

    std::cout << "Find Bridge 2 3 4" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeID " << node.first << std::endl;
        node.second->findToBRn(4);
        std::cout << std::endl;
    }
    std::cout << "Done find bridge 2 3 4" << std::endl;
    std::cout << std::endl;

    std::cout << "Broadcast Bridges to overwhelmed Locality" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeiD " << node.first << std::endl;
        for (std::vector<std::pair<int, Node*> > bridge : node.second->getOwnBridges()) {
            node.second->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
        }
        std::cout << std::endl;
    }
    std::cout << "Done Broadcast Bridges to overwhelmed Locality" << std::endl;
    std::cout << std::endl;

    std::cout << "Handle missing bridge" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        if (node.second->getCentered()) {
            std::cout << "nodeID " << node.first << std::endl;
            node.second->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
            std::cout << std::endl;
        }
    }
    std::cout << "Done Handle missing bridge" << std::endl;
    std::cout << std::endl;

    std::cout << "Update block routing table" << std::endl;
    for (std::pair<int, Node*> node : nodeList) {
        std::cout << "nodeID " << node.first << std::endl;
        node.second->updateBlockTable(numBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        std::cout << std::endl;
    }
    std::cout << "Done Update block routing table" << std::endl;
    std::cout << std::endl;

    return  0;

}