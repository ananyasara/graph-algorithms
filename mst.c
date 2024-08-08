#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "../graphutils.h" // header for functions to load and free adjacencyList

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
    AdjacencyListNode *adjacencyList;
    size_t graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);
    if (graphNodeCount == 0) {
        fprintf(stderr, "Failed to read input file\n");
        return EXIT_FAILURE;
    }

    // An array that keeps track of who is the parent node of each graph node we visit
    // In Prim's algorithm, this parents array keeps track of what is the edge that connects a node to the MST.
    graphNode_t *parents = calloc(graphNodeCount, sizeof(graphNode_t));
    for (size_t i = 0; i < graphNodeCount; i++) {
        parents[i] = -1; // -1 indicates that a node is not yet visited; i.e., node not yet connected to MST.
    }

    graphNode_t root = rand() % graphNodeCount;
    parents[root] = root;

    // Prim's algorithm:
    // A greedy algorithm that builds the minimum spanning tree.
    // For a graph with N nodes, the minimum spanning tree will have N-1 edges spanning all nodes.
    // Prim's algorithm starts with all nodes unconnected.
    // At each iteration of Prim's algorithm, the minimum weight node that connects an unconnected node to the connected set of nodes is added to the MST.
    for (unsigned iter = 0; iter < graphNodeCount - 1; iter++) {

        double minWeight = DBL_MAX; // If we find an edge with weight less than this minWeight, and edge connects a new node to MST, then mark this as the minimum weight to beat.
        graphNode_t minSource = -1;
        graphNode_t minDest = -1;

        for (graphNode_t source = 0; source < graphNodeCount; source++) {
            if (parents[source] != -1) { // if already visited
                AdjacencyListNode *dest = adjacencyList[source].next;
                while (dest) {
                    if (parents[dest->graphNode] == -1 && dest->weight < minWeight) {
                        minWeight = dest->weight;
                        minSource = source;
                        minDest = dest->graphNode;
                    }
                    dest = dest->next;
                }
            }
        }
        parents[minDest] = minSource; // we found the minimum weight
    }

   // Using the fully populated parents array, print the edges in the MST.
for (size_t i = 0; i < graphNodeCount; i++) {
    if (parents[i] != -1 && i != parents[i]) { // Avoid printing self-loops and edges with invalid node indices
        printf("%zu %zu\n", i, parents[i]); // Print i before parents[i]
    }
}



    free(parents);
    freeAdjList(graphNodeCount, adjacencyList);

    return EXIT_SUCCESS;
}
