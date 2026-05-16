#include <iostream>
#include <vector>
#include <utility>


struct node {
    char name;
    bool visited;
    int x;
    int y;

    node(char name, int x, int y):
        name(name),
        visited(false),
        x(x),
        y(y) {}
};


std::vector<std::vector<std::pair<node, int>>> adjacencyList;

void connect(node base, node other, int weight);


int main() {
    return 0;
}

void connect(node base, node other, int weight) {
    for (int i = 0; i < adjacencyList.size(); i++) {
        // Check if the node exists in the adjacency list
        if (adjacencyList[i][0].first.name == base.name) {
            adjacencyList[i].push_back({other, weight});
            return;
        }
    }

    std::vector<std::pair<node, int>> newRow = {{base, 0}, {other, weight}};
    adjacencyList.push_back(newRow);
    return;
}