#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <algorithm>
using namespace std;

typedef pair<int, int> Point;

struct Node {
    Point point;
    double f, g, h;
    Node* parent;

    Node(Point p, double g_val, double h_val, Node* par = nullptr)
        : point(p), g(g_val), h(h_val), f(g + h), parent(par) {}
};

struct CompareNode {
    bool operator()(Node* a, Node* b) {
        return a->f > b->f;
    }
};

double distance(Point a, Point b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

bool isValid(Point p, vector<vector<int>>& grid) {
    return p.first >= 0 && p.first < grid.size() && 
           p.second >= 0 && p.second < grid[0].size() && 
           grid[p.first][p.second] == 0;
}

vector<Point> getNeighbors(Point p) {
    vector<Point> neighbors = {{p.first + 1, p.second}, {p.first - 1, p.second},
                               {p.first, p.second + 1}, {p.first, p.second - 1}};
    return neighbors;
}

vector<Point> reconstructPath(Node* endNode) {
    vector<Point> path;
    Node* current = endNode;
    while (current) {
        path.push_back(current->point);
        current = current->parent;
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<Point> astar(vector<vector<int>>& grid, Point start, Point end) {
    //used to get nodes with net cost the lowest
    priority_queue<Node*, vector<Node*>, CompareNode> openList;
    //store the nodes
    unordered_map<int, Node*> allNodes;
    
    Node* startNode = new Node(start, 0, distance(start, end));
    openList.push(startNode);
    allNodes[start.first * grid[0].size() + start.second] = startNode;

    while (!openList.empty()) {
        Node* current = openList.top();
        openList.pop();

        if (current->point == end) {
            vector<Point> path = reconstructPath(current);
            for (auto node : allNodes)
                delete node.second;
            return path;
        }

        vector<Point> neighbors = getNeighbors(current->point);
        for (Point  neighbor : neighbors) {
            if (!isValid(neighbor, grid))
                continue;

            double tentative_g = current->g + 1;
            int index = neighbor.first * grid[0].size() + neighbor.second;
            if (allNodes.find(index) == allNodes.end() || tentative_g < allNodes[index]->g) {
                Node* neighborNode = new Node(neighbor, tentative_g, distance(neighbor, end), current);
                openList.push(neighborNode);
                allNodes[index] = neighborNode;
            }
        }
    }
    for (auto node : allNodes)
        delete node.second;
    return {};
}

int main() {
    vector<vector<int>> map = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 1, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    Point start = {0, 0};
    Point end = {4, 4};

    vector<Point> path = astar(map, start, end);

    if (!path.empty()) {
        cout << "Path found: ";
        for (auto p : path) {
            cout << "(" << p.first << "," << p.second << ") ";
        }
        cout << endl;
    } else {
        cout << "No path found" << endl;
    }

    return 0;
}
