/*
 * Graph algorithms.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include <limits.h>

#include "graph.h"
#include "minheap.h"

/*
 * A structure to keep record of the current running algorithm.
 */
typedef struct records
{
  int numVertices;   // total number of vertices in the graph
                     // vertex IDs are 0, 1, ..., numVertices-1
  MinHeap *heap;     // priority queue
  bool *finished;    // finished[id] is true iff vertex id is finished
                     //   i.e. no longer in the PQ
  int *predecessors; // predecessors[id] is the predecessor of vertex id
  Edge *tree;        // keeps edges for the resulting tree
  int numTreeEdges;  // current number of edges in mst
  int *distances;    // Array to store current shortest distances
} Records;

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/
/*
 * Creates, populates, and returns a MinHeap to be used by Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
MinHeap *initHeap(Graph *graph, int startVertex)
{
  MinHeap *heap = newHeap(graph->numVertices);
  insert(heap, 0, startVertex);
  for (int i = 0; i < graph->numVertices; i++)
  {
    if (i != startVertex)
    {
      insert(heap, INT_MAX, i);
    }
  }
  return heap;
}

/*
 * Creates, populates, and returns all records needed to run Prim's and
 * Dijkstra's algorithms on Graph 'graph' starting from vertex with ID
 * 'startVertex'.
 * Precondition: 'startVertex' is valid in 'graph'
 */
Records *initRecords(Graph *graph, int startVertex)
{
  Records *records = (Records *)malloc(sizeof(Records));
  if (!records)
  {
    return NULL;
  }

  records->numVertices = graph->numVertices;
  records->heap = initHeap(graph, startVertex);
  if (!records->heap)
  {
    free(records);
    return NULL;
  }

  records->finished = (bool *)calloc(graph->numVertices, sizeof(bool));
  if (!records->finished)
  {
    deleteHeap(records->heap);
    free(records);
    return NULL;
  }

  records->predecessors = (int *)malloc(graph->numVertices * sizeof(int));
  if (!records->predecessors)
  {
    free(records->finished);
    deleteHeap(records->heap);
    free(records);
    return NULL;
  }

  records->tree = (Edge *)malloc((graph->numVertices - 1) * sizeof(Edge));
  if (!records->tree)
  {
    free(records->predecessors);
    free(records->finished);
    deleteHeap(records->heap);
    free(records);
    return NULL;
  }

  records->distances = (int *)malloc(graph->numVertices * sizeof(int));
  if (!records->distances)
  {
    free(records->tree);
    free(records->predecessors);
    free(records->finished);
    deleteHeap(records->heap);
    free(records);
    return NULL;
  }

  records->numTreeEdges = 0;
  for (int i = 0; i < graph->numVertices; i++)
  {
    records->predecessors[i] = NOTHING;
    records->distances[i] = (i == startVertex) ? 0 : INT_MAX;
  }

  return records;
}

/*
 * Returns true iff 'heap' is NULL or is empty.
 */
bool isEmpty(MinHeap *heap)
{
  return heap->size == 0;
}

/*
 * Prints the status of all current algorithm data: good for debugging.
 */
void printRecords(Records *records);

/*
 * Add a new edge to records at index ind.
 */
void addTreeEdge(Records *records, int ind, int fromVertex, int toVertex,
                 int weight)
{
  records->tree[ind].fromVertex = fromVertex;
  records->tree[ind].toVertex = toVertex;
  records->tree[ind].weight = weight;
  records->numTreeEdges++;
}

/*
 * Creates and returns a path from 'vertex' to 'startVertex' from edges
 * in the distance tree 'distTree'.
 */
EdgeList *makePath(Edge *distTree, int vertex, int startVertex)
{
  EdgeList *path = NULL;
  while (vertex != startVertex)
  {
    for (int i = 0; distTree[i].fromVertex != NOTHING; i++)
    {
      if (distTree[i].toVertex == vertex)
      {
        path = newEdgeList(&distTree[i], path);
        vertex = distTree[i].fromVertex;
        break;
      }
    }
  }
  return path;
}

void cleanupRecords(Records *records)
{
  if (records)
  {
    if (records->heap)
    {
      deleteHeap(records->heap);
      records->heap = NULL;
    }
    if (records->finished)
    {
      free(records->finished);
      records->finished = NULL;
    }
    if (records->predecessors)
    {
      free(records->predecessors);
      records->predecessors = NULL;
    }
    if (records->tree)
    {
      free(records->tree);
      records->tree = NULL;
    }
    if (records->distances)
    {
      free(records->distances);
      records->distances = NULL;
    }
    free(records);
  }
}

/*************************************************************************
 ** Required functions
 *************************************************************************/
Edge *getMSTprim(Graph *graph, int startVertex)
{
  if (startVertex < 0 || startVertex >= graph->numVertices)
  {
    return NULL;
  }

  Records *records = initRecords(graph, startVertex);
  if (records == NULL)
  {
    printf("Initialization of records failed.\n");
    return NULL;
  }

  while (!isEmpty(records->heap))
  {
    HeapNode minNode = extractMin(records->heap);
    int u = minNode.id;
    records->finished[u] = true;

    EdgeList *adj = graph->vertices[u]->adjList;
    while (adj != NULL)
    {
      int v = adj->edge->toVertex;
      int weight = adj->edge->weight;
      if (!records->finished[v] && getPriority(records->heap, v) > weight)
      {
        decreasePriority(records->heap, v, weight);
        records->predecessors[v] = u;
      }
      adj = adj->next;
    }

    if (u != startVertex)
    {
      for (EdgeList *adj = graph->vertices[records->predecessors[u]]->adjList; adj != NULL; adj = adj->next)
      {
        if (adj->edge->toVertex == u)
        {
          addTreeEdge(records, records->numTreeEdges, u, records->predecessors[u], adj->edge->weight);
          break;
        }
      }
    }
  }

  Edge *mst = (Edge *)malloc(records->numTreeEdges * sizeof(Edge));
  if (mst == NULL)
  {
    printf("Memory allocation for MST failed.\n");
    cleanupRecords(records);
    return NULL;
  }

  for (int i = 0; i < records->numTreeEdges; i++)
  {
    mst[i] = records->tree[i];
  }

  cleanupRecords(records);

  return mst;
}

Edge *getDistanceTreeDijkstra(Graph *graph, int startVertex)
{
  if (startVertex < 0 || startVertex >= graph->numVertices)
  {
    return NULL;
  }

  Records *records = initRecords(graph, startVertex);
  if (records == NULL)
  {
    printf("Initialization of records failed.\n");
    return NULL;
  }

  while (!isEmpty(records->heap))
  {
    HeapNode minNode = extractMin(records->heap);
    int u = minNode.id;
    records->finished[u] = true;

    EdgeList *adj = graph->vertices[u]->adjList;
    int currentDist = records->distances[u];

    while (adj != NULL)
    {
      int v = adj->edge->toVertex;
      int weight = adj->edge->weight;
      int newDist = currentDist + weight;

      if (!records->finished[v] && records->distances[v] > newDist)
      {
        records->distances[v] = newDist;
        decreasePriority(records->heap, v, newDist);
        records->predecessors[v] = u;
      }
      adj = adj->next;
    }

    if (u != startVertex)
    {
      for (EdgeList *adj = graph->vertices[records->predecessors[u]]->adjList; adj != NULL; adj = adj->next)
      {
        if (adj->edge->toVertex == u)
        {
          addTreeEdge(records, records->numTreeEdges, records->predecessors[u], u, adj->edge->weight);
          break;
        }
      }
    }
  }

  Edge *distTree = (Edge *)malloc(records->numTreeEdges * sizeof(Edge));
  if (distTree == NULL)
  {
    printf("Memory allocation for distance tree failed.\n");
    cleanupRecords(records);
    return NULL;
  }

  for (int i = 0; i < records->numTreeEdges; i++)
  {
    distTree[i] = records->tree[i];
  }

  cleanupRecords(records);

  return distTree;
}

EdgeList **getShortestPaths(Edge *distTree, int numVertices, int startVertex)
{
  if (startVertex < 0 || startVertex >= numVertices)
  {
    return NULL;
  }

  EdgeList **paths = (EdgeList **)malloc(numVertices * sizeof(EdgeList *));
  for (int i = 0; i < numVertices; i++)
  {
    paths[i] = makePath(distTree, i, startVertex);
  }

  return paths;
}

/*************************************************************************
 ** Provided helper functions -- part of starter code to help you debug!
 *************************************************************************/
void printRecords(Records *records)
{
  if (records == NULL)
    return;

  int numVertices = records->numVertices;
  printf("Reporting on algorithm's records on %d vertices...\n", numVertices);

  printf("The PQ is:\n");
  printHeap(records->heap);

  printf("The finished array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->finished[i]);

  printf("The predecessors array is:\n");
  for (int i = 0; i < numVertices; i++)
    printf("\t%d: %d\n", i, records->predecessors[i]);

  printf("The TREE edges are:\n");
  for (int i = 0; i < records->numTreeEdges; i++)
    printEdge(&records->tree[i]);

  printf("... done.\n");
}
