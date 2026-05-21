#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
using namespace std;


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

// Based on the latest 'Graph-v#.drawio.png'
node A = node('A', 6, 10);
node B = node('B', 7, 3);
node C = node('C', 16, 11);
node D = node('D', 4, 8);
node E = node('E', 12, 2);
node F = node('F', 2, 4);
node G = node('G', 15, 4);
node H = node('H', 17, 6);
node I = node('I', 11, 7);
node J = node('J', 11, 12);

std::vector<std::vector<std::pair<node*, int>>> adjacencyList;


void connect(node& base, node& other);
int getNodeDistance(const node& base, const node& other);
void constructGraph();
void traverse(node& start, node& end);

int main() {
    constructGraph();
    
    traverse(A, E);

    return 0;
}

void traverse(node& start, node& end){
    node* current = &start;
    while(current->name != end.name) {
        cout << current->name << " -> ";
        int minG = INT_MAX;
        int minH = INT_MAX;
        int minSum = INT_MAX;
        node* nextNode = current;
        current->visited = true;

        for (int i = 0; i < adjacencyList.size(); i++) {
            if (adjacencyList[i][0].first->name == current->name) {

                for (int j = 1; j < adjacencyList[i].size(); j++) {
                    node* currentNode = adjacencyList[i][j].first;
                    int gCost = adjacencyList[i][j].second;
                    int dCost = getNodeDistance(*currentNode, end);

                    int sumCost = gCost + dCost;

                    if (!currentNode->visited && sumCost < minSum) {
                        minG = gCost;
                        minH = dCost;
                        minSum = sumCost;
                        currentNode->visited = true;
                        nextNode = currentNode;
                    }
                }
                break;
            }
        }

        if (nextNode == current) {
            cout << "No path found!" << endl;
            return;
        }

        current = nextNode;
    }
    cout << end.name << endl;
}

void connect(node& base, node& other) {
    int weight = getNodeDistance(base, other);

    for (int i = 0; i < adjacencyList.size(); i++) {
        // Check if the node exists in the adjacency list
        if (adjacencyList[i][0].first->name == base.name) {
            adjacencyList[i].push_back({&other, weight});
            return;
        }
    }

    std::vector<std::pair<node*, int>> newRow = {{&base, 0}, {&other, weight}};
    adjacencyList.push_back(newRow);
    return;
}

// Rounds up to the nearest whole number
int getNodeDistance(const node& base, const node& other) {
    float x = pow((base.x - other.x), 2);
    float y = pow((base.y - other.y), 2);
    return x + y;
}

// Based on the latest 'Graph-v#.drawio.png'
void constructGraph() {
    connect(A, B);
    connect(A, C);
    connect(B, A);
    connect(B, D);
    connect(B, E);
    connect(C, A);
    connect(C, D);
    connect(C, F);
    connect(D, B);
    connect(D, C);
    connect(D, E);
    connect(D, F);
    connect(E, B);
    connect(E, D);
    connect(E, G);
    connect(F, C);
    connect(F, D);
    connect(F, G);
    connect(F, H);
    connect(G, E);
    connect(G, F);
    connect(G, I);
    connect(H, F);
    connect(H, I);
    connect(H, J);
    connect(I, G);
    connect(I, H);
    connect(I, J);
    connect(J, H);
    connect(J, I);
}