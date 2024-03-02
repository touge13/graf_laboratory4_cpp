#include "../include/algorithm.h"

//-----------------------нормальное расположение вершин------------

//Функция для преобразования списка рёбер в список смежности
std::vector<std::vector<int> > edgesToAdjacencyList(const std::vector<Edge> edges) {
    int maxVertex = -1;
    for (const auto& edge : edges) {
        maxVertex = std::max(maxVertex, std::max(edge.u, edge.v));
    }

    //Увеличим размер adjacencyList до максимального номера вершины + 1
    std::vector<std::vector<int> > adjacencyList(maxVertex + 1);
    for (const auto& edge : edges) {
        adjacencyList[edge.u].push_back(edge.v);
        adjacencyList[edge.v].push_back(edge.u);
    }

    return adjacencyList;
}

void fruchtermanReingold::operator()(std::vector<Point>& positions) {
    planeVector start;
    start.dx = 0;
    start.dy = 0;

    fill(power.begin(), power.end(), start);

    // Сила отталкивания между парами вершин
    for (int v_id = 0; v_id < adj_list.size(); v_id++) {
        for (size_t other_id = v_id + 1; other_id < adj_list.size(); ++other_id) {
            planeVector delta;
            delta.dx = positions[v_id].x - positions[other_id].x;
            delta.dy = positions[v_id].y - positions[other_id].y;
            double distance = delta.norm();
            if (distance != 0.0) {
                double repulsion = k * k / distance;

                power[v_id].dx += delta.dx / distance * repulsion;
                power[v_id].dy += delta.dy / distance * repulsion;
                power[other_id].dx -= delta.dx / distance * repulsion;
                power[other_id].dy -= delta.dy / distance * repulsion;
            }
        }
    }

    // Вычисляем притягивающие силы
    for(size_t v_id = 0; v_id < adj_list.size(); ++v_id) {
        for (size_t adj_id : adj_list[v_id]) {
            if (adj_id > v_id) {
                planeVector delta;
                delta.dx = positions[v_id].x - positions[adj_id].x;
                delta.dy = positions[v_id].y - positions[adj_id].y;
                double distance = delta.norm();
                if (distance != 0.0) {
                    double attraction = distance * distance / k;
                    power[v_id].dx -= delta.dx / distance * attraction;
                    power[v_id].dy -= delta.dy / distance * attraction;
                    power[adj_id].dx += delta.dx / distance * attraction;
                    power[adj_id].dy += delta.dy / distance * attraction;
                }
            }
        }
    }

    // Максимальное движение ограничено текущей температурой
    for (int v_id = 0; v_id < adj_list.size(); v_id++) {
        double powerNorm = power[v_id].norm();
        // < 1.0: не стоит вычислять
        if (powerNorm < 1.0) {
            continue;
        }
        double capped_powerNorm = std::min(powerNorm, temp);
        planeVector cappedPower;
        cappedPower.dx = power[v_id].dx / powerNorm * capped_powerNorm;
        cappedPower.dy = power[v_id].dy / powerNorm * capped_powerNorm;

        positions[v_id].x += cappedPower.dx;
        positions[v_id].y += cappedPower.dy;
    }

// Быстро остываем, пока не достигнем 1,5, затем остаемся при низкой температуре
    if (temp > 1) {
        temp *= 0.97;
    } else {
        temp = 1;
    }
}

//-----------------------центруем и масштабируем граф-----------------

void centerGraph(int V, std::vector<Point>& vertexCoords, int width, int height){
    //Находим максимальные и минимальные значения координат всех вершин
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    for (int i = 0; i < V; ++i) {
        if (vertexCoords[i].x < minX) {
            minX = vertexCoords[i].x;
        }
        if (vertexCoords[i].x > maxX) {
            maxX = vertexCoords[i].x;
        }
        if (vertexCoords[i].y < minY) {
            minY = vertexCoords[i].y;
        }
        if (vertexCoords[i].y > maxY) {
            maxY = vertexCoords[i].y;
        }
    }

    //Находим центр графа
    double centerX = (maxX + minX) / 2;
    double centerY = (maxY + minY) / 2;
    
    //Смещаем центр графа в координаты (width/2,height/2)
    double offsetX = (width/2) - centerX;
    double offsetY = (height/2) - centerY;

    //Смещаем граф
    for (int i = 0; i < V; ++i) {
        double newX = vertexCoords[i].x + offsetX;
        double newY = vertexCoords[i].y + offsetY;
        vertexCoords[i].x = newX;
        vertexCoords[i].y = newY;
    }
}

void scaleGraph(int V, std::vector<Point>& vertexCoords, int width, int height, int vertexSize){
    //Находим максимальные и минимальные значения координат всех вершин
    int minX = INT_MAX;
    int minY = INT_MAX;
    int maxX = INT_MIN;
    int maxY = INT_MIN;

    for (int i = 0; i < V; ++i) {
        if (vertexCoords[i].x < minX) {
            minX = vertexCoords[i].x;
        }
        if (vertexCoords[i].x > maxX) {
            maxX = vertexCoords[i].x;
        }
        if (vertexCoords[i].y < minY) {
            minY = vertexCoords[i].y;
        }
        if (vertexCoords[i].y > maxY) {
            maxY = vertexCoords[i].y;
        }
    }

    //Находим центр графа
    double centerX = (maxX + minX) / 2;
    double centerY = (maxY + minY) / 2;

    //Проверяем, выйдет ли граф за пределы изображения
    int flag = 3;
    for (int i = 0; i < V; ++i) {
        double newX_3 = centerX + (vertexCoords[i].x - centerX) * 3;
        double newY_3 = centerY + (vertexCoords[i].y - centerY) * 3;
        if ((newX_3 + vertexSize/2) > width || (newX_3 - vertexSize/2) < 0 || (newY_3 + vertexSize/2) > height || (newY_3 - vertexSize/2) < 0) {
            flag = 2;
            break;
        }
    }
    for (int i = 0; i < V; ++i) {
        double newX_2 = centerX + (vertexCoords[i].x - centerX) * 2;
        double newY_2 = centerY + (vertexCoords[i].y - centerY) * 2;
        if ((newX_2 + vertexSize/2) > width || (newX_2 - vertexSize/2) < 0 || (newY_2 + vertexSize/2) > height || (newY_2 - vertexSize/2) < 0) {
            flag = 1;
            break;
        }
    }

    //Масштабируем граф
    if (flag == 3) {
        for (int i = 0; i < V; ++i) {
            double newX = centerX + (vertexCoords[i].x - centerX) * 3;
            double newY = centerY + (vertexCoords[i].y - centerY) * 3;
            vertexCoords[i].x = newX;
            vertexCoords[i].y = newY;
        }
    } else if (flag == 2) {
        for (int i = 0; i < V; ++i) {
            double newX = centerX + (vertexCoords[i].x - centerX) * 2;
            double newY = centerY + (vertexCoords[i].y - centerY) * 2;
            vertexCoords[i].x = newX;
            vertexCoords[i].y = newY;
        }
    }
}

fruchtermanReingold::fruchtermanReingold(const std::vector<std::vector<int> >& g, double k)
    : adj_list(g)
    , k(k)
    , kSquared(k * k)
    , temp(10 * sqrt(g.size()))
    , power(adj_list.size()) {}