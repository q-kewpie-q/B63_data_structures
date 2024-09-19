/*
 * Some light testing of our MinHeap implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich and F. Estrada
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minheap.h"

#define MAX_LIMIT 1024
#define DEFAULT_CAPACITY 50

MinHeap* createHeap(FILE* f);
void testHeap(MinHeap* heap);
void printHeapReport(MinHeap* heap);

int main(int argc, char* argv[])
{
  MinHeap* heap = NULL;

  // If user specified a file for reading, create a heap with priorities from it.
  if (argc > 1)
  {
    FILE* f = fopen(argv[1], "r");
    if (f == NULL)
    {
      fprintf(stderr, "Unable to open the specified input file: %s\n", argv[1]);
      exit(0);
    }
    heap = createHeap(f);
    fclose(f);
  }
  else
  {
    printf("You did not specify an input file.");
    printf(" We will start with an empty heap of default capacity %d.\n",
           DEFAULT_CAPACITY);
    heap = newHeap(DEFAULT_CAPACITY);
  }

  testHeap(heap);
  return 0;
}

MinHeap* createHeap(FILE* f)
{
  char line[MAX_LIMIT];

  // Create a new Heap with given capacity (must be first line in file)
  fgets(line, MAX_LIMIT, f);
  int capacity = atoi(line);
  printf("Creating Heap with capacity %d...\n", capacity);
  MinHeap* heap = newHeap(capacity);

  int priority = 0;
  int id = 0;
  while (fgets(line, MAX_LIMIT, f)) // read the next line
  {
    priority = atoi(line);
    printf("read %d\n", priority);
    insert(heap, priority, id++);  // assign IDs sequentially
    printHeapReport(heap);
  }
  return heap;
}

void testHeap(MinHeap* heap)
{
  char line[MAX_LIMIT];
  HeapNode node;
  int priority = 0;
  int id = 0;
  int index = 0;

  while (1)
  {
    printf("Choose a command: (g)et-min, (e)xtract-min, (i)nsert, ");
    printf("(c)hange-priority, (q)uit\n");
    fgets(line, MAX_LIMIT, stdin);
    if (line[0] == 'q') {  // quit
      printf("quit selected. Goodbye!\n");
      deleteHeap(heap);
      return;
    }
    if (line[0] == 'g') // get-min
    {
      printf("get-min selected.\n");
      if (heap->size == 0)
      {
        printf("Heap is empty: can't get min. Choose another command.\n");
        continue;
      }
      node = getMin(heap);
      printf("Minimum is priority %d of node with ID %d.\n", node.priority,
             node.id);
    }
    else if (line[0] == 'e') // extract-min
    {
      printf("extract-min selected.\n");
      if (heap->size == 0)
      {
        printf("Heap is empty: can't extract min. Choose another command.\n");
        continue;
      }
      node = extractMin(heap);
      printf("Minimum was priority %d of node with ID %d.\n", node.priority,
             node.id);
      printHeapReport(heap);
    }
    else if (line[0] == 'i') // insert
    {
      if (heap->size == heap->capacity)
      {
        printf("Heap is full: can't insert. Choose another command.\n");
        continue;
      }
      printf("insert selected. Enter priority to insert: ");
      fgets(line, MAX_LIMIT, stdin);
      priority = atoi(line);
      printf("Enter ID for this node (must be unique): ");
      fgets(line, MAX_LIMIT, stdin);
      id = atoi(line);
      if (insert(heap, priority, id))
        printf("Insert successful\n");
      else
        printf("Insert unsuccesful\n");
      printHeapReport(heap);
    }
    else if (line[0] == 'c') // change-priority
    {
      printf("change-priority selected. Enter node's heap index: ");
      fgets(line, MAX_LIMIT, stdin);
      index = atoi(line);
      printf("change-priority selected. Enter node new priority: ");
      fgets(line, MAX_LIMIT, stdin);
      changePriority(heap, index, atoi(line));
      printHeapReport(heap);
    }
  }
}

void printHeapReport(MinHeap* heap)
{
  printf("** The heap is now:\n");
  printHeap(heap);
  printf("**\n");
}
