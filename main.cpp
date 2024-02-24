#include <iostream>
#include <fstream>
#include <vector>

#include "createImage.hpp"

int main() {
    std::ifstream inputFile("input.txt"); // file with input data

    int V, E;
    inputFile >> V >> E;
    
    std::vector<Edge> edges;
    edges.reserve(E);

    for(int i = 0; i < E; i++){
        int u, v;
        inputFile >> u >> v;
        edges.push_back(Edge(u, v));
    }

    createImage(V, edges);    
    return 0;
}