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
    int cost;
    node* parent;

    node(char name, int x, int y):
        name(name),
        visited(false),
        x(x),
        y(y),
        cost(INT_MAX),
        parent(nullptr) {}
};

// Based on the latest 'Graph-v#.drawio.png'
node* A = new node('A', 6, 10);
node* B = new node('B', 7, 3);
node* C = new node('C', 16, 11);
node* D = new node('D', 4, 8);
node* E = new node('E', 12, 2);
node* F = new node('F', 2, 4);
node* G = new node('G', 15, 4);
node* H = new node('H', 17, 6);
node* I = new node('I', 11, 7);
node* J = new node('J', 11, 12);

vector<vector<pair<node*, int>>> adjacencyList;
// vector<node*> nodes = {A, B, C, D, E, F, G, H, I, J};
vector<node*> minheap = {new node('Z', 0, 0)};


void connect(node* base, node* other);
int getNodeDistance(node* base, node* other);
void constructGraph();
void traverse(node* start, node* end);
void pushMinHeap(node* node);
node* popMinHeap();
void traceBackPath(node* node);
void printMinHeap();

int main(int argc, char* argv[]) {
    constructGraph();

    node* start = A;
    node* end = J;

    traverse(start, end);

    cout << "Path from " << start->name << " to " << end->name << "\n";
    traceBackPath(end);
    cout << endl;
    
    return 0;
}

void printMinHeap() {
    for (int i = 1; i < minheap.size(); i++) {
        cout << minheap[i]->name << " ";
    }
    cout << endl;
}

void traceBackPath(node* node) {
    if (node->parent != nullptr) {
        traceBackPath(node->parent);
    }
    cout << node->name << " ";
}

void traverse(node* start, node* end) {
    start->cost = 0;
    pushMinHeap(start);
    node* currentNode;

    while (minheap.size() > 1) {
        printMinHeap(); // DEBUG
        
        currentNode = popMinHeap();
        if (currentNode == nullptr) {
            cout << "No path found!\n";
            return;
        }
        
        if (currentNode->visited) continue;

        currentNode->visited = true; 

        // Find the edges of the current node
        for (vector row : adjacencyList) {
            if (row[0].first->name == currentNode->name) {

                for (int i = 1; i < row.size(); i++) {
                    node* neighbor = row[i].first;
                    int weight = row[i].second;

                    int newCost = currentNode->cost + weight;

                    if (newCost < neighbor->cost) {
                        neighbor->cost = currentNode->cost + weight;
                        neighbor->parent = currentNode;
                        pushMinHeap(neighbor);
                    }
                }
                break;
            }
        }
    }
}

node* popMinHeap() {
    if (minheap.size() < 2) return nullptr;

    node* root = minheap[1];
    minheap[1] = minheap.back();
    minheap.pop_back();

    int i = 1;
    int maxIndex = minheap.size() - 1;

    while (i*2 < maxIndex) {
        int left = i*2;
        int right = i*2 + 1;

        if (minheap[i]->cost <= minheap[left]->cost && minheap[i]->cost <= minheap[right]->cost) break;

        if (minheap[left]->cost < minheap[right]->cost) {
            node* temp = minheap[i];
            minheap[i] = minheap[left];
            minheap[left] = temp;
    
            i = left;
        } else if (minheap[left]->cost > minheap[right]->cost) {
            node* temp = minheap[i];
            minheap[i] = minheap[right];
            minheap[right] = temp;
    
            i = right;
        }
    }

    return root;
}

void pushMinHeap(node* node) {
    minheap.push_back(node);
    int i = minheap.size() - 1;

    while (i > 1 && node->cost < minheap[i/2]->cost) {
        minheap[i] = minheap[i/2];
        minheap[i/2] = node;
        i = i/2;
    }
}

void connect(node* base, node* other) {
    int weight = getNodeDistance(base, other);
    // cout << base->name << " -" << weight << "->" << other->name << endl;

    for (int i = 0; i < adjacencyList.size(); i++) {
        // Check if the node exists in the adjacency list
        if (adjacencyList[i][0].first->name == base->name) {
            adjacencyList[i].push_back({other, weight});
            return;
        }
    }

    vector<pair<node*, int>> newRow = {{base, 0}, {other, weight}};
    adjacencyList.push_back(newRow);
    return;
}

// Rounds up to the nearest whole number
int getNodeDistance(node* base, node* other) {
    float x = pow((base->x - other->x), 2);
    float y = pow((base->y - other->y), 2);
    return ceil(sqrt(x + y));
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