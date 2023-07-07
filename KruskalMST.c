#include <stdio.h>
#include <stdlib.h>

// Structure to represent an edge in the graph
typedef struct {
  int source;      // Source vertex of the edge
  int destination; // Destination vertex of the edge
  int weight;      // Weight of the edge
} Edge;

// Structure to represent a graph
typedef struct {
  int numVertices; // Number of vertices in the graph
  int numEdges;    // Number of edges in the graph
  Edge *edges;     // Array of edges in the graph
} Graph;

// Function to create a graph with a given number of vertices and edges
Graph *createGraph(int numVertices, int numEdges) {
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  graph->numVertices = numVertices;
  graph->numEdges = numEdges;
  graph->edges = (Edge *)malloc(numEdges * sizeof(Edge));
  return graph;
}

// Function to compare two edges based on their weights (used for sorting)
int compareEdges(const void *a, const void *b) {
  Edge *edgeA = (Edge *)a;
  Edge *edgeB = (Edge *)b;
  return edgeA->weight - edgeB->weight;
}

// Function to find the parent of a vertex (with path compression)
int findParent(int *parent, int vertex) {
  if (parent[vertex] != vertex) {
    // Path compression: Assign the vertex's parent as the root of the set
    parent[vertex] = findParent(parent, parent[vertex]);
  }
  return parent[vertex];
}

// Function to perform union of two sets by rank (union by rank)
void unionSets(int *parent, int *rank, int vertex1, int vertex2) {
  int root1 = findParent(parent, vertex1);
  int root2 = findParent(parent, vertex2);

  if (rank[root1] < rank[root2]) {
    parent[root1] = root2;
    rank[root2]++;
  } else if (rank[root1] > rank[root2]) {
    parent[root2] = root1;
    rank[root1]++;
  } else {
    parent[root2] = root1;
    rank[root1]++;
  }
}

// Function to find the minimum spanning tree using Kruskal's algorithm
void kruskalMST(Graph *graph) {
  int numVertices = graph->numVertices;
  Edge *edges = graph->edges;

  // Sort the edges in non-decreasing order of their weights
  qsort(edges, graph->numEdges, sizeof(Edge), compareEdges);

  // Allocate memory for parent and rank arrays
  int *parent = (int *)malloc(numVertices * sizeof(int));
  int *rank = (int *)malloc(numVertices * sizeof(int));

  // Initialize parent and rank arrays
  for (int i = 0; i < numVertices; i++) {
    parent[i] = i;
    rank[i] = 0;
  }

  // Array to store the edges in the minimum spanning tree
  Edge *mst = (Edge *)malloc((numVertices - 1) * sizeof(Edge));
  int mstCount = 0; // Number of edges in the minimum spanning tree

  // Process each edge in non-decreasing order of their weights
  for (int i = 0; i < graph->numEdges; i++) {
    Edge currentEdge = edges[i];
    int root1 = findParent(parent, currentEdge.source);
    int root2 = findParent(parent, currentEdge.destination);

    // If including the current edge does not form a cycle, add it to the
    // minimum spanning tree
    if (root1 != root2) {
      mst[mstCount] = currentEdge;
      mstCount++;
      unionSets(parent, rank, root1, root2);
    }
  }

  // Print the minimum spanning tree
  printf("Minimum Spanning Tree:\n");
  for (int i = 0; i < mstCount; i++) {
    printf("%d - %d : %d\n", mst[i].source, mst[i].destination, mst[i].weight);
  }

  // Free memory
  free(parent);
  free(rank);
  free(mst);
}

// Sample usage of Kruskal's algorithm
int main() {
  int numVertices = 4;
  int numEdges = 5;
  Graph *graph = createGraph(numVertices, numEdges);

  // Add edges to the graph
  graph->edges[0].source = 0;
  graph->edges[0].destination = 1;
  graph->edges[0].weight = 10;

  graph->edges[1].source = 0;
  graph->edges[1].destination = 2;
  graph->edges[1].weight = 6;

  graph->edges[2].source = 0;
  graph->edges[2].destination = 3;
  graph->edges[2].weight = 5;

  graph->edges[3].source = 1;
  graph->edges[3].destination = 3;
  graph->edges[3].weight = 15;

  graph->edges[4].source = 2;
  graph->edges[4].destination = 3;
  graph->edges[4].weight = 4;

  kruskalMST(graph);

  // Free memory
  free(graph->edges);
  free(graph);

  return 0;
}
