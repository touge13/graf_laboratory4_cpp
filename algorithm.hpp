#include <vector>
#include <cmath>

struct Edge {
    int u;
    int v;
    Edge(int u, int v) : u(u), v(v) {}
};

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

// planeVector in plane
struct planeVector {
    int dx, dy;

    planeVector() : dx(0), dy(0) {}
    planeVector(double x, double y) : dx(x), dy(y) {};

    double norm() const {
        return sqrt(dx * dx + dy * dy);
    }
};

struct Point {
    int x;
    int y;
    
    Point() : x(0), y(0) {}
    Point(double x, double y) : x(x), y(y) {}
};

class fruchtermanReingold {
public:
    fruchtermanReingold(std::vector<std::vector<int> > adj_list, double k = 30.0);
    void operator()(std::vector<Point>& positions);

private:
    const std::vector<std::vector<int> > adj_list;
    const double k;
    const double kSquared;
    double temp;
    std::vector<Edge> edges;
    std::vector<planeVector> power;
};

fruchtermanReingold::fruchtermanReingold(const std::vector<std::vector<int> > g, double k)
    : adj_list(g)
    , k(k)
    , kSquared(k * k)
    , temp(10 * sqrt(g.size()))
    , power(adj_list.size()) {}

void fruchtermanReingold::operator()(std::vector<Point>& positions) {
    planeVector start;
    start.dx = 0;
    start.dy = 0;

    fill(power.begin(), power.end(), start);

    // Repulsion force between vertice pairs
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

    // Max movement capped by current temperature
    for (int v_id = 0; v_id < adj_list.size(); v_id++) {
        double powerNorm = power[v_id].norm();
        // < 1.0: not worth computing
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

    // Cool down fast until we reach 1.5, then stay at low temperature
    if (temp > 1) {
        temp *= 0.97;
    } else {
        temp = 1;
    }
}