/*
 * Our graph implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "graph.h"

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/

void printEdge(Edge *edge)
{
  if (edge == NULL)
    printf("NULL");
  else
    printf("(%d -- %d, %d)", edge->fromVertex, edge->toVertex, edge->weight);
}

void printEdgeList(EdgeList *head)
{
  while (head != NULL)
  {
    printEdge(head->edge);
    printf(" --> ");
    head = head->next;
  }
  printf("NULL");
}

void printVertex(Vertex *vertex)
{
  if (vertex == NULL)
  {
    printf("NULL");
  }
  else
  {
    printf("%d: ", vertex->id);
    printEdgeList(vertex->adjList);
  }
}

void printGraph(Graph *graph)
{
  if (graph == NULL)
  {
    printf("NULL");
    return;
  }
  printf("Number of vertices: %d. Number of edges: %d.\n\n", graph->numVertices,
         graph->numEdges);

  for (int i = 0; i < graph->numVertices; i++)
  {
    printVertex(graph->vertices[i]);
    printf("\n");
  }
  printf("\n");
}

/*********************************************************************
 ** Required functions
 *********************************************************************/

Edge *newEdge(int fromVertex, int toVertex, int weight)
{
  Edge *edge = (Edge *)malloc(sizeof(Edge));
  if (!edge)
  {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  edge->fromVertex = fromVertex;
  edge->toVertex = toVertex;
  edge->weight = weight;
  return edge;
}

EdgeList *newEdgeList(Edge *edge, EdgeList *next)
{
  EdgeList *edgeList = (EdgeList *)malloc(sizeof(EdgeList));
  if (!edgeList)
  {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  edgeList->edge = edge;
  edgeList->next = next;
  return edgeList;
}

Vertex *newVertex(int id, void *value, EdgeList *adjList)
{
  Vertex *vertex = (Vertex *)malloc(sizeof(Vertex));
  if (!vertex)
  {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  vertex->id = id;
  vertex->value = value;
  vertex->adjList = adjList;
  return vertex;
}

Graph *newGraph(int numVertices)
{
  Graph *graph = (Graph *)malloc(sizeof(Graph));
  if (!graph)
  {
    printf("Memory allocation failed\n");
    exit(EXIT_FAILURE);
  }
  graph->numVertices = numVertices;
  graph->numEdges = 0;
  graph->vertices = (Vertex **)malloc(numVertices * sizeof(Vertex *));
  if (!graph->vertices)
  {
    printf("Memory allocation failed\n");
    free(graph);
    exit(EXIT_FAILURE);
  }
  for (int i = 0; i < numVertices; i++)
  {
    graph->vertices[i] = NULL;
  }
  return graph;
}

void deleteEdgeList(EdgeList *head)
{
  while (head != NULL)
  {
    EdgeList *temp = head;
    head = head->next;
    free(temp->edge);
    free(temp);
  }
}

void deleteVertex(Vertex *vertex)
{
  if (vertex)
  {
    deleteEdgeList(vertex->adjList);
    free(vertex);
  }
}

void deleteGraph(Graph *graph)
{
  if (graph)
  {
    for (int i = 0; i < graph->numVertices; i++)
    {
      deleteVertex(graph->vertices[i]);
    }
    free(graph->vertices);
    free(graph);
  }
}
