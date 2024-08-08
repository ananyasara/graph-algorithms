#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../graphutils.h" // header for functions to load and free adjacencyList

#define INVALID_NODE -1 // Define INVALID_NODE as -1 or any other appropriate value

// A recursive function that returns true if no cycles found
bool isTreeDFS (
   size_t graphNodeCount,
   AdjacencyListNode* adjacencyList,
   bool* visited,
   graphNode_t parent,
   graphNode_t current
) {
   // First see if current node has already been visited, indicating a cycle found
   if (visited[current]) {
       return false; // Cycle found
   }

   // Current node was not already visited, so now mark it as visited
   visited[current] = true;

   // Now iterate through each of the neighboring graph nodes
   AdjacencyListNode* neighbor = adjacencyList[current].next;
   while (neighbor) {
       if (neighbor->graphNode != parent) {
           // If the neighbor node is not the parent node (the node from which we arrived at current), call DFS
           if (!isTreeDFS(graphNodeCount, adjacencyList, visited, current, neighbor->graphNode)) {
               return false; // Cycle found in the neighbor's subtree
           }
       }
       neighbor = neighbor->next;
   }

   // All DFS searches from current node found no cycles, so graph is a tree from this node
   return true;
}

int main ( int argc, char* argv[] ) {
   if (argc != 2) {
       fprintf(stderr, "Usage: %s <input_file>\n", argv[0]);
       return EXIT_FAILURE;
   }

   // READ INPUT FILE TO CREATE GRAPH ADJACENCY LIST
   AdjacencyListNode* adjacencyList = NULL;
   size_t graphNodeCount = 0;

   graphNodeCount = adjMatrixToList(argv[1], &adjacencyList);
   if (graphNodeCount == 0) {
       fprintf(stderr, "Failed to read input file\n");
       return EXIT_FAILURE;
   }

   // Array of boolean variables indicating whether graph node has been visited
   bool* visited = calloc(graphNodeCount, sizeof(bool));
   if (!visited) {
       fprintf(stderr, "Memory allocation failed\n");
       freeAdjList(graphNodeCount, adjacencyList);
       return EXIT_FAILURE;
   }

   // Check if the graph is a tree using DFS
   bool isTree = isTreeDFS(graphNodeCount, adjacencyList, visited, INVALID_NODE, 0);

   // Output result
   printf(isTree ? "yes\n" : "no\n");

   // Free allocated memory
   free(visited);
   freeAdjList(graphNodeCount, adjacencyList);

   return EXIT_SUCCESS;
}
