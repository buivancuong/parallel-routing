//
// Created by cuongbv on 13/11/2019.
//

#include <iostream>
#include "Graph.h"

Graph::Graph(int totalNode) {
    this->totalNode = totalNode;
}

void Graph::addVertice(Vertice newVertice) {
    this->totalNode++;
    this->verticesList.insert(std::pair<int, Vertice>(newVertice.getVerticeID(), newVertice));
}

void Graph::addEdge(int source, int dest) {
    if ((this->verticesList.count(source) == 0) or (this->verticesList.count(dest) == 0)) {
        std::cout << "Invalid of source or destination verticeID" << std::endl;
        return;
    }
    // Check exited status of Edge
    if (this->adjList[dest].count(source) != 0) {
        Edge *edge = this->adjList[dest][source];
        this->adjList[source].insert(std::pair<int, Edge*>(dest, edge));
    } else {
        // Create new Edge
        Edge *newEdge = new Edge(std::pair<int, int>(source, dest));
        // Add the new Edge to adjList an both of source and dest
        this->adjList[source].insert(std::pair<int, Edge*>(dest, newEdge));
        this->adjList[dest].insert(std::pair<int, Edge*>(source, newEdge));
    }
}

int Graph::getTotalNode() {
    return this->totalNode;
}

std::map<int, std::map<int, Edge*> > Graph::getAdjList() {
    return this->adjList;
}

std::map<int, Edge*> Graph::getNeighbors(int verticeID) {
    return this->adjList[verticeID];
}

Graph::Graph() {
    this->totalNode = 0;
};

Graph::~Graph() = default;
