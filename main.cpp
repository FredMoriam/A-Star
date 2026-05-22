/*
    A* Implementation in C++

    - Nodes have (x, y) coordinates which can and are used
    to calculate edge weights and heuristic based on distance

    - Instead of two sets, a priority queue is used along with
    a boolean node.visited field

    Graphs available: (refer to the included jpegs)
        1. 'Graph-v1'
            - Weights are calculated based on actual distance between nodes
        2. 'Graph-v2'
            - weights were randomly given

    Additional notes before running the program:
        i. Choose a graph to use in the main() function
        ii. A debug option to print the nodes in the priority queue and their
            costs is available in the traverse() function
*/


#include <iostream>
#include <vector>
#include <utility>
#include <cmath>
#include <climits>

using namespace std;


struct node {
    char name;
    bool visited;
    int x;
    int y;
    int gCost;
    int fCost;
    node* parent;

    node(char name, int x, int y):
        name(name),
        visited(false),
        x(x),
        y(y),
        gCost(INT_MAX),
        fCost(INT_MAX),
        parent(nullptr) {}
};

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
vector<node*> nodes = {A, B, C, D, E, F, G, H, I, J};
vector<node*> minheap = {new node('Z', 0, 0)};


void connect(node* base, node* other);
int getNodeDistance(node* base, node* other);
void constructGraph();
void traverse(node* start, node* end);
void pushMinHeap(node* node);
node* popMinHeap();
void traceBackPath(node* node);
void printMinHeap();
void connectWithWeight(node* base, node* other, int weight);
void constructGraphV2();


int main(int argc, char* argv[]) {
    // !=== CHANGE GRAPH HERE ===!
    // constructGraph();           // Refer to Graph-v1.drawio.jpg
    constructGraphV2();         // Refer to Graph-v2.drawio.jpg

    if (argc != 3) {
        cout << "Run the program with the following arguments: <start_node> <end_node>\n";
        cout << "Ex: \"<.exe> A J\"\n";
        return 1;
    }

    string inputStartNode = argv[1];
    string inputEndNode = argv[2];
    node* start;
    node* end;

    for (node* node : nodes) {
        if (node->name == *argv[1]) start = node;
        else if (node->name == *argv[2]) end = node;
    }

    traverse(start, end);

    cout << "Path from " << start->name << " to " << end->name << "\n";
    traceBackPath(end);
    cout << " | Cost: " << end->gCost;
    cout << endl;
    
    return 0;
}

void printMinHeap() {
    for (int i = 1; i < minheap.size(); i++) {
        cout << minheap[i]->name << "(" << minheap[i]->gCost << ") ";
    }
    cout << endl;
}

// Recursive printing of a path from start to another node
void traceBackPath(node* node) {
    if (node->parent != nullptr) {
        traceBackPath(node->parent);
    }
    cout << node->name << " ";
}

void traverse(node* start, node* end) {
    start->gCost = 0;
    start->fCost = 0;
    pushMinHeap(start);
    node* currentNode = nullptr;

    while (true) {
        // !=== UNCOMMENT TO SEE HEAP AND NODE COSTS ===!
        // printMinHeap();

        currentNode = popMinHeap();
        
        if (currentNode == nullptr) {
            cout << "No path found!\n";
            return;
        }

        if (currentNode->name == end->name) break;

        if (currentNode->visited) continue;

        currentNode->visited = true; 

        // Find the edges of the current node
        for (auto& row : adjacencyList) {
            if (row[0].first->name == currentNode->name) {

                // Update each neighbor
                for (int i = 1; i < row.size(); i++) {
                    node* neighbor = row[i].first;
                    int weight = row[i].second;

                    // A-Star implementation
                    int newCost = currentNode->gCost + weight;

                    if (newCost < neighbor->gCost) {
                        neighbor->gCost = newCost;
                        neighbor->fCost = newCost + getNodeDistance(neighbor, end);
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

    while (i*2 <= maxIndex) {
        int left = i*2;
        int right = i*2 + 1;

        // If right child exists, then the left child also exists
        if (right <= maxIndex) {
            if (minheap[i]->fCost <= minheap[left]->fCost && minheap[i]->fCost <= minheap[right]->fCost) break;

            if (minheap[left]->fCost < minheap[right]->fCost) {
                node* temp = minheap[i];
                minheap[i] = minheap[left];
                minheap[left] = temp;
        
                i = left;
            } else {
                node* temp = minheap[i];
                minheap[i] = minheap[right];
                minheap[right] = temp;
        
                i = right;
            }

        // Only the left child exists
        } else if (left <= maxIndex) {
            if (minheap[i]->fCost < minheap[left]->fCost)
                break;
            else {
                node* temp = minheap[i];
                minheap[i] = minheap[left];
                minheap[left] = temp;
        
                i = left;
            }
        }
    }

    return root;
}

void pushMinHeap(node* node) {
    minheap.push_back(node);
    int i = minheap.size() - 1;

    while (i > 1 && node->fCost < minheap[i/2]->fCost) {
        minheap[i] = minheap[i/2];
        minheap[i/2] = node;
        i = i/2;
    }
}

void connect(node* base, node* other) {
    int weight = getNodeDistance(base, other);

    connectWithWeight(base, other, weight);
}

void connectWithWeight(node* base, node* other, int weight) {
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

// Follows 'Graph-v1.drawio.jpg'
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

// Follows 'Graph-v2.drawio.jpg'
void constructGraphV2() {
    connectWithWeight(A, D, 12);
    connectWithWeight(A, I, 14);
    connectWithWeight(A, J, 10);
    connectWithWeight(B, D, 5);
    connectWithWeight(B, E, 6);
    connectWithWeight(B, F, 13);
    connectWithWeight(B, I, 8);
    connectWithWeight(C, H, 7);
    connectWithWeight(C, J, 6);
    connectWithWeight(D, A, 12);
    connectWithWeight(D, B, 5);
    connectWithWeight(D, F, 2);
    connectWithWeight(E, B, 6);
    connectWithWeight(E, G, 17);
    connectWithWeight(F, B, 13);
    connectWithWeight(F, D, 2);
    connectWithWeight(G, E, 17);
    connectWithWeight(G, H, 4);
    connectWithWeight(G, I, 15);
    connectWithWeight(H, C, 7);
    connectWithWeight(H, G, 4);
    connectWithWeight(H, I, 3);
    connectWithWeight(I, A, 14);
    connectWithWeight(I, B, 8);
    connectWithWeight(I, G, 15);
    connectWithWeight(I, H, 3);
    connectWithWeight(I, J, 8);
    connectWithWeight(J, A, 10);
    connectWithWeight(J, C, 6);
    connectWithWeight(J, I, 8);
}