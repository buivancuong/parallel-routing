#include <iostream>
#include <cstdio>
#include <ctime>
#include <cmath>
#include <thread>
#include <pthread.h>
#include "graph/smallworld/SmallWorld2DGrid.h"
#include "node/Node.h"
#include "utils/CORRAUtils.h"


// ######### CORRA v2 #########

// --------- Serial ---------
//int main() {
//    int xTopoSize = 64;
//    int yTopoSize = 64;
//    int deltaNeighbor = 3;
//    std::vector<float> alphas = {1.6, 2};
//
//    int xBlockSize, yBlockSize;
//    if ((int)(log2(xTopoSize)) % 2 == 0) {
//        xBlockSize = (int) sqrt(xTopoSize);
//    } else {
//        xBlockSize = (int) sqrt(xTopoSize / 2);
//    }
//    if ((int) (log2(yTopoSize)) % 2 == 0) {
//        yBlockSize = (int) sqrt(yTopoSize);
//    } else {
//        yBlockSize = (int) sqrt(yTopoSize / 2);
//    }
//    int numBlock = (xTopoSize/xBlockSize) * (yTopoSize/yBlockSize);
//
//    std::cout << xBlockSize << " " << yBlockSize << std::endl;
//
//    // Create Topology
//    auto *topo = new SmallWorld2DGrid(yTopoSize, xTopoSize, alphas);
//
//    // Create list of Nodes on Topo
//    std::map<int, Node*> nodeList;
//    for (int i = 0; i < xTopoSize * yTopoSize; ++i) {
//        Node *node = new Node(i);
//        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
//        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//        if (centerVertex == i) {
//            node->setCentered(true);
//            std::cout << "center " << centerVertex << std::endl;
//        }
//        nodeList.insert(std::pair<int, Node*>(i, node));
//    }
//
//    // Add near neighbors (grid neighbor) and far neighbors (random link neighbors)
//    for (std::pair<int, std::map<int, float> > sourceNode : topo->getAdjList()) {
//        std::cout << "Node " << sourceNode.first << std::endl;
//        for (std::pair<int, float> neighbor : sourceNode.second) {
//            if (neighbor.second == 1) {     // grid neighbor
//                std::cout << "add near " << neighbor.first << std::endl;
//                nodeList[sourceNode.first]->addNearNeighbors(nodeList[neighbor.first]);
//            } else {        // random link neighbor
//                std::cout << "add far " << neighbor.first << std::endl;
//                nodeList[sourceNode.first]->addFarNeighbors(nodeList[neighbor.first]);
//            }
//        }
//        std::cout << std::endl;
//    }
//
//    std::cout << "Prepare for create the Locality" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeID " << node.first << std::endl;
//        node.second->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//        std::cout << std::endl;
//    }
//    std::cout << "*******************************" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Create the Locality" << std::endl;
//    for (std::pair<int, Node*> node: nodeList) {
//        std::cout << "nodeiD " << node.first << std::endl;
//        node.second->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//        std::cout << std::endl;
//    }
//    std::cout << "Done create the locality" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Update local routing table" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeID " << node.first << std::endl;
//        node.second->createLocalRouting(xTopoSize, yTopoSize);
//        std::cout << std::endl;
//    }
//    std::cout << "Done Update local routing table" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Find Bridge 1" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeID " << node.first << std::endl;
//        node.second->findBR1();
//        std::cout << std::endl;
//    }
//    std::cout << "Done find bridge 1" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Find Bridge 2 3 4" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeID " << node.first << std::endl;
//        node.second->findToBRn(4);
//        std::cout << std::endl;
//    }
//    std::cout << "Done find bridge 2 3 4" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Broadcast Bridges to overwhelmed Locality" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeiD " << node.first << std::endl;
//        for (std::vector<std::pair<int, Node*> > bridge : node.second->getOwnBridges()) {
//            node.second->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
//        }
//        std::cout << std::endl;
//    }
//    std::cout << "Done Broadcast Bridges to overwhelmed Locality" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Handle missing bridge" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        if (node.second->getCentered()) {
//            std::cout << "nodeID " << node.first << std::endl;
//            node.second->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//            std::cout << std::endl;
//        }
//    }
//    std::cout << "Done Handle missing bridge" << std::endl;
//    std::cout << std::endl;
//
//    std::cout << "Update block routing table" << std::endl;
//    for (std::pair<int, Node*> node : nodeList) {
//        std::cout << "nodeID " << node.first << std::endl;
//        node.second->updateBlockTable(numBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
//        std::cout << std::endl;
//    }
//    std::cout << "Done Update block routing table" << std::endl;
//    std::cout << std::endl;
//
//    return  0;
//
//}

//--------- Parallel ---------
std::map<int, Node*> nodeList;

void createNodeList(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    std::cout << "Parallellizing create node list procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        Node *node = new Node(i);
        std::cout << "nodeID " << i << std::endl;
        int nodeBlock = CORRAUtils::getNodeBlock(i, xBlockSize, yBlockSize, xTopoSize);
        int centerVertex = CORRAUtils::getCenterVertex(nodeBlock, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
        if (centerVertex == i) {
            node->setCentered(true);
//            std::cout << "center " << centerVertex << std::endl;
        }
        nodeList.insert(std::pair<int, Node*>(i, node));
    }
}

void createNearFarNeighbors(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize,int yTopoSize) {
    std::cout << "Parallelizing Prepare for create the Locality procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->prepareLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void createLocality(int startNodeID, int endNodeID, int deltaNeighbor, int xBlockSize, int yBlockSize, int xTopoSize,int yTopoSize) {
    std::cout << "Parallelizing Create the Locality procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->createLocality(deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void createLocalRouting(int startNodeID, int endNodeID, int xTopoSize) {
    std::cout << "Parallelizing Create the Local Routing Table procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->createLocalRouting(xTopoSize);
    }
}

void findBR1(int startNodeID, int endNodeID) {
    std::cout << "Parallelizing Find bridge 1 procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->findBR1();
    }
}

void findBRn(int startNodeID, int endNodeID, int n) {
    std::cout << "Parallelizing Find bridge n procedure" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->findToBRn(n);
    }
}

void broadcastLocalBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize) {
    std::cout << "Parallelizing Broadcast Local Bridges" << std::endl;
    for (int i = startNodeID; i <= endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->broadcastLocalBridge(xBlockSize, yBlockSize, xTopoSize);
    }
}

void handleMissingBridge(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    std::cout << "Parallelizing Handle missing bridges" << std::endl;
    for (int i = startNodeID; i < endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->handleMissingBridge(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}

void updateBlockTable(int startNodeID, int endNodeID, int xBlockSize, int yBlockSize, int xTopoSize, int yTopoSize) {
    std::cout << "Parallelizing Update block table " << std::endl;
    for (int i = startNodeID; i < endNodeID; ++i) {
        std::cout << "nodeID " << i << std::endl;
        nodeList[i]->updateBlockTable(xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    }
}


int main() {

    int xTopoSize = 64;
    int yTopoSize = 64;
    int deltaNeighbor = 4;
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

    std::thread thread11(createNodeList, 0, 1023, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread12(createNodeList, 1024, 2047, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread13(createNodeList, 2048, 3071, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread14(createNodeList, 3072, 4095, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

    thread11.join();
    thread12.join();
    thread13.join();
    thread14.join();

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

    std::cout << "Prepare Locality 0" << std::endl;
    std::thread thread21 (createNearFarNeighbors, 0, 1023, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread22 (createNearFarNeighbors, 1024, 2047, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread23 (createNearFarNeighbors, 2048, 3071, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread24 (createNearFarNeighbors, 3072, 4095, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

    thread21.join();
    thread22.join();
    thread23.join();
    thread24.join();


    std::cout << "Create the Locality" << std::endl;
    std::thread thread31 (createLocality, 0, 1023, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread32 (createLocality, 2014, 2047, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread33 (createLocality, 2048, 3071, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread34 (createLocality, 3072, 4095, deltaNeighbor, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

    thread31.join();
    thread32.join();
    thread33.join();
    thread34.join();


    std::cout << "Create the Local Routing table" << std::endl;
    std::thread thread41 (createLocalRouting, 0, 1023, xTopoSize);
    std::thread thread42 (createLocalRouting, 2014, 2047, xTopoSize);
    std::thread thread43 (createLocalRouting, 2048, 3071, xTopoSize);
    std::thread thread44 (createLocalRouting, 3072, 4095, xTopoSize);

    thread41.join();
    thread42.join();
    thread43.join();
    thread44.join();


    std::cout << "Find the bridge 1 " << std::endl;
    std::thread thread51 (findBR1, 0, 1023);
    std::thread thread52 (findBR1, 2014, 2047);
    std::thread thread53 (findBR1, 2048, 3071);
    std::thread thread54 (findBR1, 3072, 4095);

    thread51.join();
    thread52.join();
    thread53.join();
    thread54.join();

    std::cout << "Find the bridge n " << std::endl;
    std::thread thread61 (findBRn, 0, 1023, 4);
    std::thread thread62 (findBRn, 2014, 2047, 4);
    std::thread thread63 (findBRn, 2048, 3071, 4);
    std::thread thread64 (findBRn, 3072, 4095, 4);

    thread61.join();
    thread62.join();
    thread63.join();
    thread64.join();


    std::cout << "Broadcast Local Bridges " << std::endl;
    std::thread thread71 (broadcastLocalBridge, 0, 1023, xBlockSize, yBlockSize, xTopoSize);
    std::thread thread72 (broadcastLocalBridge, 2014, 2047, xBlockSize, yBlockSize, xTopoSize);
    std::thread thread73 (broadcastLocalBridge, 2048, 3071, xBlockSize, yBlockSize, xTopoSize);
    std::thread thread74 (broadcastLocalBridge, 3072, 4095, xBlockSize, yBlockSize, xTopoSize);

    thread71.join();
    thread72.join();
    thread73.join();
    thread74.join();

    std::cout << "Handle missing bridges " << std::endl;
    std::thread thread81 (handleMissingBridge, 0, 1023, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread82 (handleMissingBridge, 2014, 2047, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread83 (handleMissingBridge, 2048, 3071, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread84 (handleMissingBridge, 3072, 4095, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

    thread81.join();
    thread82.join();
    thread83.join();
    thread84.join();

    std::cout << "Update block table " << std::endl;
    std::thread thread91 (updateBlockTable, 0, 1023, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread92 (updateBlockTable, 2014, 2047, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread93 (updateBlockTable, 2048, 3071, xBlockSize, yBlockSize, xTopoSize, yTopoSize);
    std::thread thread94 (updateBlockTable, 3072, 4095, xBlockSize, yBlockSize, xTopoSize, yTopoSize);

    thread91.join();
    thread92.join();
    thread93.join();
    thread94.join();


    return 0;
}