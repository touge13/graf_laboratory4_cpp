#include "../include/createImage.h"
#include "../include/imageConfigurations.h"

//функция для отрисовки графа с корректно расставленными вершинами
void createResultImage (int V, std::vector<Edge> edges, std::vector<uint8_t>& pixels, std::vector<Point>& vertexCoords, int width, int height, int vertexSize) {
    // Определение общего количество шагов выполнения программы
    int totalSteps = V + edges.size();
    int currentStep = 0;

    // Отрисовка рёбер
    for (const auto& edge : edges) {
        int u = edge.u, v = edge.v;
        drawLine(pixels, vertexCoords[u].x, vertexCoords[u].y, vertexCoords[v].x, vertexCoords[v].y, width, height);
        
        //Прогресс выполнения
        currentStep++;
        float completionPercentage = static_cast<float>(currentStep) / totalSteps * 100;
        if (currentStep % 2 == 0) {
            std::cout << "Completion: " << std::fixed << std::setprecision(1) << completionPercentage << "%" << std::endl;
        }
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

        //Прогресс выполнения
        currentStep++;
        float completionPercentage = static_cast<float>(currentStep) / totalSteps * 100;
        if (currentStep % 2 == 0) {
            std::cout << "Completion: " << std::fixed << std::setprecision(1) << completionPercentage << "%" << std::endl;
        }
    }
}

void createImage(int V, std::vector<Edge> edges){
    //uint8_t = 1 байт
    //размер вектора ширина * высота * размер каждого пикселя (ргб => 3 байта)
    std::vector<uint8_t> pixels(width * height * 3);

    std::ofstream file("../conversion/output.bmp", std::ios::out | std::ios::binary);

    // BMP Header (шаблонно)
    file << "BM"; 
    file.write(reinterpret_cast<char*>(&fileSize), 4); 
    std::string reserver = "\0\0\0\0"; 
    file.write(reinterpret_cast<char*>(&reserver), 4); 
    file.write(reinterpret_cast<char*>(&dataOffset), 4); 
    file.write(reinterpret_cast<char*>(&headerSize), 4); 
    file.write(reinterpret_cast<char*>(&width), 4); 
    file.write(reinterpret_cast<char*>(&height), 4); 
    file.write(reinterpret_cast<char*>(&planes), 2); 
    file.write(reinterpret_cast<char*>(&bpp), 2); 
    file.write(reinterpret_cast<char*>(&compression), 4); 
    file.write(reinterpret_cast<char*>(&dataSize), 4); 
    file.write(reinterpret_cast<char*>(&hResolution), 4); 
    file.write(reinterpret_cast<char*>(&vResolution), 4); 
    file.write(reinterpret_cast<char*>(&colors), 4); 
    file.write(reinterpret_cast<char*>(&importantColors), 4); 

    // BMP data
    // Заливаем фон
    for (int i = 0; i < width * height; ++i) {
        pixels[i * 3] = backgroundColorB;     // B
        pixels[i * 3 + 1] = backgroundColorG; // G
        pixels[i * 3 + 2] = backgroundColorR; // R
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
    for (int i = 0; i < numIteration; i++){
        algorithm(vertexCoords);
    }
    
    // Отрисовка и сохранение
    centerGraph(V, vertexCoords, height, width);
    scaleGraph(V, vertexCoords, width, height, vertexSize);
    createResultImage(V, edges, pixels, vertexCoords, width, height, vertexSize);
    saveFile(width, height, file, pixels);
    
}