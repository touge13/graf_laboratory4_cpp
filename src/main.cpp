// cd "/Users/admin/Desktop/work/С++/graf_laboratory4_cpp/src/" && g++ -std=c++11 main.cpp -o main && "/Users/admin/Desktop/work/С++/graf_laboratory4_cpp/src/"main

#include <iostream>
#include <fstream>
#include <vector>

#include "../include/createImage.h"

int main() {
    std::ifstream inputFile("../conversion/input.txt"); // файл со входными данными

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