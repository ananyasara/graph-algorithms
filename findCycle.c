#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_NODES 100 // Assuming maximum number of nodes in the graph

typedef struct AdjacencyListNode {
    int dest;
    struct AdjacencyListNode* next;
} AdjacencyListNode;

AdjacencyListNode* createNode(int dest) {
    AdjacencyListNode* newNode = (AdjacencyListNode*)malloc(sizeof(AdjacencyListNode));
    newNode->dest = dest;
    newNode->next = NULL;
    return newNode;
}

void addEdge(AdjacencyListNode** adjacencyList, int src, int dest) {
    AdjacencyListNode* newNode = createNode(dest);
    newNode->next = adjacencyList[src];
    adjacencyList[src] = newNode;
}

bool dfs(int node, bool* visited, bool* stack, AdjacencyListNode** adjacencyList, int* parent) {
    visited[node] = true;
    stack[node] = true;

    AdjacencyListNode* neighbor = adjacencyList[node];
    while (neighbor != NULL) {
        if (!visited[neighbor->dest]) {
            parent[neighbor->dest] = node;
            if (dfs(neighbor->dest, visited, stack, adjacencyList, parent))
                return true;
        } else if (stack[neighbor->dest]) {
            // Cycle detected
            int cur = node;
            while (cur != neighbor->dest) {
                printf("%d ", cur);
                cur = parent[cur];
            }
            printf("%d\n", neighbor->dest);
            return true;
        }
        neighbor = neighbor->next;
    }

    stack[node] = false;
    return false;
}

void freeAdjacencyList(AdjacencyListNode** adjacencyList, int graphNodeCount) {
    for (int i = 0; i < graphNodeCount; i++) {
        AdjacencyListNode* current = adjacencyList[i];
        while (current != NULL) {
            AdjacencyListNode* temp = current;
            current = current->next;
            free(temp);
        }
    }
}

void findCycle(int graphNodeCount, AdjacencyListNode** adjacencyList) {
    bool visited[MAX_NODES] = {false};
    bool stack[MAX_NODES] = {false};
    int parent[MAX_NODES];

    for (int i = 0; i < graphNodeCount; i++) {
        if (!visited[i]) {
            if (dfs(i, visited, stack, adjacencyList, parent))
                return;
        }
    }

    printf("DAG\n");
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* inputFile = fopen(argv[1], "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file.\n");
        return EXIT_FAILURE;
    }

    int graphNodeCount;
    fscanf(inputFile, "%d", &graphNodeCount);

    AdjacencyListNode* adjacencyList[MAX_NODES] = {NULL};

    for (int i = 0; i < graphNodeCount; i++) {
        for (int j = 0; j < graphNodeCount; j++) {
            int isConnected;
            fscanf(inputFile, "%d", &isConnected);
            if (isConnected)
                addEdge(adjacencyList, i, j);
        }
    }

    fclose(inputFile);

    findCycle(graphNodeCount, adjacencyList);
    freeAdjacencyList(adjacencyList, graphNodeCount);

    return EXIT_SUCCESS;
}