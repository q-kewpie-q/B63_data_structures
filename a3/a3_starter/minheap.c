/*
 * Our Priority Queue implementation.
 *
 * Author: Akshay Arun Bapat
 * Based on implementation from A. Tafliovich
 */

#include "minheap.h"

#define ROOT_INDEX 1
#define NOTHING -1

/*************************************************************************
 ** Suggested helper functions -- part of starter code
 *************************************************************************/

/*
 * Returns True if 'nodeIndex' is currently a valid index in minheap 'heap'.
 * False otherwise.
 */
bool isValidIndex(MinHeap *heap, int nodeIndex)
{
    return nodeIndex >= ROOT_INDEX && nodeIndex <= heap->size;
}

/*
 * Swaps contents of heap->arr[index1] and heap->arr[index2] if both
 * 'index1' and 'index2' are valid indices for minheap 'heap'.
 * Has no effect otherwise.
 */
void swap(MinHeap *heap, int index1, int index2)
{
    if (!isValidIndex(heap, index1) || !isValidIndex(heap, index2))
    {
        return;
    }
    HeapNode temp = heap->arr[index1];
    heap->arr[index1] = heap->arr[index2];
    heap->arr[index2] = temp;

    heap->indexMap[heap->arr[index1].id] = index1;
    heap->indexMap[heap->arr[index2].id] = index2;
}

/*
 * Returns priority of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 *               'heap' is non-empty
 */
int priorityAt(MinHeap *heap, int nodeIndex)
{
    return heap->arr[nodeIndex].priority;
}

/*
 * Returns the index of the parent of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getParentIdx(int nodeIndex)
{
    return nodeIndex / 2;
}

/*
 * Floats up the element at index 'nodeIndex' in minheap 'heap' such that
 * 'heap' is still a minheap.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
void floatUp(MinHeap *heap, int nodeIndex)
{
    while (nodeIndex > ROOT_INDEX)
    {
        int parentIdx = getParentIdx(nodeIndex);
        if (priorityAt(heap, nodeIndex) < priorityAt(heap, parentIdx))
        {
            swap(heap, nodeIndex, parentIdx);
            nodeIndex = parentIdx;
        }
        else
        {
            break;
        }
    }
}

/*
 * Returns the index of the left child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getLeftChildIdx(int nodeIndex)
{
    return 2 * nodeIndex;
}

/*
 * Returns the index of the right child of a node at index 'nodeIndex',
 * assuming it exists.
 * Precondition: 'nodeIndex' is a valid index of minheap 'heap'
 */
int getRightChildIdx(int nodeIndex)
{
    return 2 * nodeIndex + 1;
}

/*
 * Returns node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
HeapNode nodeAt(MinHeap *heap, int nodeIndex)
{
    return heap->arr[nodeIndex];
}

/*
 * Returns ID of node at index 'nodeIndex' in minheap 'heap'.
 * Precondition: 'nodeIndex' is a valid index in 'heap'
 *               'heap' is non-empty
 */
int idAt(MinHeap *heap, int nodeIndex)
{
    return heap->arr[nodeIndex].id;
}

/*
 * Returns index of node with ID 'id' in minheap 'heap'.
 * Precondition: 'id' is a valid ID in 'heap'
 *               'heap' is non-empty
 */
int indexOf(MinHeap *heap, int id)
{
    return heap->indexMap[id];
}

/*********************************************************************
 * Required functions
 ********************************************************************/
HeapNode getMin(MinHeap *heap)
{
    if (heap->size == 0)
    {
        printf("Heap is empty\n");
        exit(EXIT_FAILURE);
    }
    return heap->arr[ROOT_INDEX];
}

void heapify(MinHeap *heap, int nodeIndex)
{
    int smallest = nodeIndex;
    int leftChild = getLeftChildIdx(nodeIndex);
    int rightChild = getRightChildIdx(nodeIndex);

    if (leftChild <= heap->size && priorityAt(heap, leftChild) < priorityAt(heap, smallest))
    {
        smallest = leftChild;
    }
    if (rightChild <= heap->size && priorityAt(heap, rightChild) < priorityAt(heap, smallest))
    {
        smallest = rightChild;
    }

    if (smallest != nodeIndex)
    {
        swap(heap, nodeIndex, smallest);
        heapify(heap, smallest);
    }
}

HeapNode extractMin(MinHeap *heap)
{
    if (heap->size == 0)
    {
        printf("Heap is empty\n");
        exit(EXIT_FAILURE);
    }

    HeapNode minNode = getMin(heap);               // Get the root element
    heap->arr[ROOT_INDEX] = heap->arr[heap->size]; // Move the last element to root
    heap->indexMap[heap->arr[ROOT_INDEX].id] = ROOT_INDEX;
    heap->size--; // Decrement the size
    if (heap->size > 0)
    { // Only heapify if there are elements left
        heapify(heap, ROOT_INDEX);
    }

    return minNode;
}

bool insert(MinHeap *heap, int priority, int id)
{
    if (heap->size == heap->capacity)
    {
        return false;
    }

    heap->size++;
    int index = heap->size;
    heap->arr[index].priority = priority;
    heap->arr[index].id = id;
    heap->indexMap[id] = index;
    floatUp(heap, index);

    return true;
}

int getPriority(MinHeap *heap, int id)
{
    int index = indexOf(heap, id);
    // printf("index: %d, id: %d", index, id); //debug print
    if (!isValidIndex(heap, index))
    {
        printf("Invalid ID at index: %d, heap cap: %d, heap size: %d\n", index, heap->capacity, heap->size);
        exit(EXIT_FAILURE);
    }
    return priorityAt(heap, index);
}

bool decreasePriority(MinHeap *heap, int id, int newPriority)
{
    int index = indexOf(heap, id);
    if (!isValidIndex(heap, index) || priorityAt(heap, index) <= newPriority)
    {
        return false;
    }
    heap->arr[index].priority = newPriority;
    floatUp(heap, index); // Ensure the node floats up if necessary

    return true;
}

/*********************************************************************
 ** Helper function provided in the starter code
 *********************************************************************/
void printHeap(MinHeap *heap)
{
    printf("MinHeap with size: %d\n\tcapacity: %d\n\n", heap->size,
           heap->capacity);
    printf("index: priority [ID]\t ID: index\n");
    for (int i = 0; i < heap->capacity; i++)
        printf("%d: %d [%d]\t\t%d: %d\n", i, heap->arr[i].priority,
               heap->arr[i].id, i, heap->indexMap[i]);
    printf("\n\n");
}

/***** Memory management (sample solution) **********************************/
MinHeap *newHeap(int capacity)
{
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    if (!heap)
    {
        printf("Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = capacity;
    heap->arr = (HeapNode *)malloc((capacity + 1) * sizeof(HeapNode)); // Fixed allocation size
    heap->indexMap = (int *)malloc((capacity + 1) * sizeof(int));      // Fixed allocation size

    if (!heap->arr || !heap->indexMap)
    {
        printf("Memory allocation failed\n");
        free(heap);
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i <= capacity; i++)
    { // Initialize all entries
        heap->indexMap[i] = NOTHING;
    }
    return heap;
}

void deleteHeap(MinHeap *heap)
{
    if (heap)
    {
        free(heap->arr);
        free(heap->indexMap);
        free(heap);
    }
}
