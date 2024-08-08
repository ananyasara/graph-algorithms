#include "../graphutils.h" // header for functions to load and free adjacencyList

// A program to print the edge list of a graph given the adjacency matrix
int main ( int argc, char* argv[] ) {

    // FIRST, READ THE ADJACENCY MATRIX FILE
    AdjacencyListNode* adjacencyList = NULL;
    /* ... */
    size_t numNodes = adjMatrixToList(
        argv[1],
        &adjacencyList
    );
    // NEXT, TRAVERSE THE ADJACENCY LIST AND PRINT EACH EDGE, REPRESENTED AS A PAIR OF NODES
    // Example of traversing the adjacency list is in the freeAdjList() function in graphutils.h
    /* ... */

    for(size_t source=0; source<numNodes; source++)
    {
        AdjacencyListNode* dest= adjacencyList[source].next;
        //list iterator
        while(dest)
        {
            graphNode_t destNodeName=(*dest).graphNode;
            printf("%ld %ld\n", source, destNodeName);
            dest=dest->next; // iterator moves to next
        }
    }
    // NOW, BE SURE TO FREE THE ADJACENCY LIST
    /* ... */
    freeAdjList (
        numNodes,
        adjacencyList
    );

    return EXIT_SUCCESS;
}
