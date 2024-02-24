#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

#include "saveFile.hpp"
#include "drawingFigures.hpp"
#include "algorithm.hpp"

void createImage(int V, std::vector<Edge> edges){
    int width = 600;
    int height = 600;
    int vertexSize = 7; // Диаметр вершины

    //uin8_t = 1 байт
    //размер вектора ширина * высота * размер каждого пикселя (ргб => 3 байта)
    std::vector<uint8_t> pixels(width * height * 3);

    std::ofstream file("output.bmp", std::ios::out | std::ios::binary);

    // BMP Header (шаблонно)
    file << "BM"; 
    int fileSize = 54 + 3 * width * height; 
    file.write(reinterpret_cast<char*>(&fileSize), 4); 
    std::string reserver = "\0\0\0\0"; 
    file.write(reinterpret_cast<char*>(&reserver), 4); 
    int dataOffset = 54; 
    file.write(reinterpret_cast<char*>(&dataOffset), 4); 
    int headerSize = 40; 
    file.write(reinterpret_cast<char*>(&headerSize), 4); 
    file.write(reinterpret_cast<char*>(&width), 4); 
    file.write(reinterpret_cast<char*>(&height), 4); 
    short planes = 1; 
    file.write(reinterpret_cast<char*>(&planes), 2); 
    short bpp = 24; 
    file.write(reinterpret_cast<char*>(&bpp), 2); 
    int compression = 0; 
    file.write(reinterpret_cast<char*>(&compression), 4); 
    int dataSize = 3 * width * height; 
    file.write(reinterpret_cast<char*>(&dataSize), 4); 
    int hResolution = 0; 
    file.write(reinterpret_cast<char*>(&hResolution), 4); 
    int vResolution = 0; 
    file.write(reinterpret_cast<char*>(&vResolution), 4); 
    int colors = 0; 
    file.write(reinterpret_cast<char*>(&colors), 4); 
    int importantColors = 0; 
    file.write(reinterpret_cast<char*>(&importantColors), 4); 

    // BMP data
    //Белый фон
    for (int i = 0; i < width * height * 3; ++i) { 
        pixels[i] = 255; 
    }

     // вершины по кругу
    std::vector<Point> vertexCoords(V);
    for (int i = 0; i < V; ++i) {
        double angle = 2 * M_PI * i / V;
        int x = height / 2 + static_cast<int>((height / 3.5 - vertexSize / 2) * cos(angle));
        int y = height / 2 + static_cast<int>((height / 3.5 - vertexSize / 2) * sin(angle));
        vertexCoords[i] = Point(x, y);
    }

    // Стартуем алгоритм
    std::vector<std::vector<int> > adj_list = edgesToAdjacencyList(edges);
    fruchtermanReingold algorithm(adj_list);

    for (int i = 0; i < 100; i++){
        algorithm(vertexCoords);
    }

    // Отрисовка рёбер
    for (const auto& edge : edges) {
        int u = edge.u, v = edge.v;
        drawLine(pixels, vertexCoords[u].x, vertexCoords[u].y, vertexCoords[v].x, vertexCoords[v].y, width, height);
    }


    // Отрисовка вершин
    for (int i = 0; i < V; ++i) {
        // Получаем координаты центра вершины
        int centerX = vertexCoords[i].x;
        int centerY = vertexCoords[i].y;

        // Отрисовываем круг (вершину)
        drawCircle(pixels, centerX, centerY, vertexSize, width, height);

        /*
        int posx = 0;
        int posy = 0;
        if(i == 0) {
            posx = centerX;
            posy = centerY;
        }
        if(centerX >= posx && centerY >= posy) {
            drawText(pixels, centerX + 10, centerY + 10, std::to_string(i), width, height);
        }else if(centerX >= posx && centerY < posy) {
            drawText(pixels, centerX + 10, centerY - 10, std::to_string(i), width, height);
        }else if(centerX < posx && centerY >= posy) {
            drawText(pixels, centerX - 10, centerY + 10, std::to_string(i), width, height);
        }else if(centerX < posx && centerY < posy) {
            drawText(pixels, centerX - 10, centerY - 10, std::to_string(i), width, height);
        } 
        */

       drawText(pixels, centerX - 10, centerY - 20, std::to_string(i), width, height);

    }
    
    saveFile(height, width, file, pixels);
}