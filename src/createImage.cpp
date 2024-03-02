#include "../include/createImage.h"

void createResultImage (int V, std::vector<Edge> edges, std::vector<uint8_t>& pixels, std::vector<Point>& vertexCoords, int width, int height, int vertexSize) {
    // Отрисовка рёбер
    for (const auto& edge : edges) {
        int u = edge.u, v = edge.v;
        drawLine(pixels, vertexCoords[u].x, vertexCoords[u].y, vertexCoords[v].x, vertexCoords[v].y, width, height);
    }

    // Отрисовка вершин и цифр
    for (int i = 0; i < V; ++i) {
        // Получаем координаты центра вершины
        int centerX = vertexCoords[i].x;
        int centerY = vertexCoords[i].y;

        // Отрисовываем круг (вершину)
        drawCircle(pixels, centerX, centerY, vertexSize, width, height);
        // Отрисовываем номера вершин (цифры)
        drawText(pixels, centerX - 10, centerY - 20, std::to_string(vertexCoords[i].num), width, height);

    }
}

void createImage(int V, std::vector<Edge> edges){
    int width = 600;
    int height = 600;
    int vertexSize = 6; // Диаметр вершины

    //uint8_t = 1 байт
    //размер вектора ширина * высота * размер каждого пикселя (ргб => 3 байта)
    std::vector<uint8_t> pixels(width * height * 3);

    std::ofstream file("../conversion/output.bmp", std::ios::out | std::ios::binary);

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
    // Заливаем фон
    for (int i = 0; i < width * height; ++i) {
        pixels[i * 3] = (uint8_t) 255;     // B
        pixels[i * 3 + 1] = (uint8_t) 255; // G
        pixels[i * 3 + 2] = (uint8_t) 255; // R
    }
    
    // Изначально расставляем вершины по кругу
    std::vector<Point> vertexCoords(V);
    for (int i = 0; i < V; ++i) {
        double angle = 2 * M_PI * i / V;
        int x = height / 2 + static_cast<int>((height / 3.5 - vertexSize / 2) * cos(angle));
        int y = height / 2 + static_cast<int>((height / 3.5 - vertexSize / 2) * sin(angle));
        vertexCoords[i] = Point(x, y, i);
    }

    // Стартуем алгоритм
    std::vector<std::vector<int> > adj_list = edgesToAdjacencyList(edges);
    fruchtermanReingold algorithm(adj_list);

    for (int i = 0; i < 100; i++){
        algorithm(vertexCoords);
    }
    
    centerGraph(V, vertexCoords, height, width);
    scaleGraph(V, vertexCoords, width, height, vertexSize);
    createResultImage(V, edges, pixels, vertexCoords, width, height, vertexSize);
    saveFile(width, height, file, pixels);
    
}