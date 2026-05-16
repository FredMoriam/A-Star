#include <iostream>
#include <vector>


struct node {
    char name;
    bool visited;
    int x;
    int y;
    edge *edges;

    node(char name, int x, int y):
        name(name),
        x(x),
        y(y),
        visited(false),
        edges(nullptr) {}
};

struct edge {
    int weight;
    node *to;
    edge *next;

    edge(int weight, node *to):
        weight(weight),
        to(to),
        next(nullptr) {}
};


std::vector<node> adjacencyList;

void connect(node base, node other);


int main() {
    return 0;
}

void connect(node base, node other, int weight) {
    // Create an edge and connect it to the base node
    edge *newEdge = new edge(weight, &other);
    newEdge->next = base.edges;
    base.edges = newEdge;

    for (int i = 0; i < adjacencyList.size(); i++) {
        // Check if the node exists in the adjacency list
        if (adjacencyList[i].name == base.name) {
            return;
        }
    }

    adjacencyList.push_back(base);
    return;
}