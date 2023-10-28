//Algoritmos P1
//Autores:
//Pablo Manzanares López        pablo.manzanares.lopez@udc.es
//Alejandro Rodríguez Franco    alejandro.rodriguezf@udc.es
//Carlos Pérez Cambeiro         carlos.perez.cambeiro@udc.es

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <stdbool.h>

#define TAM 25600

typedef struct heap
{
    int size;
    int v[TAM];
} 
*heap_t;

void inicializar_semilla();
void swap(int *a, int *b);
int parent(int i);
int leftChild(int i);
int rightChild(int i);
int isLeaf(int i, heap_t heap);
int item(heap_t heap, int i);
int size(heap_t heap);
void initializeHeap(heap_t heap);
bool isEmptyHeap(heap_t heap);
void floatPos(heap_t heap, int i);
void sinkPos(heap_t heap, int i);
void insert(int x, heap_t heap);
int deleteMin(heap_t heap);
void createHeap(int * v, int n, heap_t heap);


void inicializar_semilla() { srand(time(NULL)); }

void swap(int *a, int *b) 
{
    int aux = *a;
    *a = *b;
    *b = aux;
}

int parent(int i) { return (i-1)/2; }

int leftChild(int i) { return 2*i+1; }

int rightChild(int i) { return 2*i+2; }

int isLeaf(int i, heap_t heap) { return rightChild(i) >= heap->size || leftChild(i) >= heap->size; }

int item(heap_t heap, int i) { return heap->v[i]; }

int size(heap_t heap) { return heap->size; }

void initializeHeap(heap_t heap) { heap->size = 0; }

bool isEmptyHeap(heap_t heap) { return !heap->size; }

void floatPos(heap_t heap, int i)
{
    while (i > 0 && item(heap, i/2) < item(heap, i)) 
    {
        swap(item(heap, i/2), item(heap, i));
        i/=2;
    }
}

void sinkPos(heap_t heap, int i)
{
    int j = i;

    do
    {
        if (!isLeaf(rightChild(i), heap) && item(heap, rightChild(i)) < item(heap, i)) i = rightChild(i);
        if (!isLeaf(leftChild(i), heap) && item(heap, leftChild(i)) < item(heap, i)) i = leftChild(i);
        swap(item(heap, i), item(heap, j));
    } 
    while (j!=i);
}

void insert(int x, heap_t heap)
{
    if (size(heap) == TAM) printf("Error: montículo lleno");
    else
    {
        size(heap) = heap->size+1;
        heap->v[heap->size] = x;
        flotar(heap, heap->size);
    }
}

int deleteMin(heap_t heap)
{
    int x = -1;

    if (estaVacioMonticulo(heap)) printf("Error: montículo vacío");
    else
    {
        x = heap->v[0];
        heap->v[0] = heap->v[heap->size];
        heap->size--;

        if (heap->size) hundir(heap, 0);
    }
    
    return x;
}

void createHeap(int * v, int n, heap_t heap)
{
    int i;

    for (i = 0; i < n; i++) heap->v[i] = v[i];

    heap->size = n;

    for (i = heap->size/2-1; i >= 0; i--) hundir(heap, i);
}