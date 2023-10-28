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

#define TAM 256000
#define MALLOC malloc(TAM*sizeof(int))

typedef struct monticulo
{
    int ultimo;
    int vector[TAM];
} 
*pmonticulo;

//rand
void inicializar_semilla();
double microsegundos();
//initializations
void initializeRandom(int * v, int n);
void initializeAsc(int * v, int n);
void initializeDesc(int * v, int n);
//print
void printArray(int * v, int n);
void tableTimes(void (*initialize) (int*, int), double n1, double n2, double n3);
//heap info
int leftChild(int i);
int rightChild(int i);
int size(pmonticulo m);
//times
double getTimesCreate(int n);
double getTimesOrder(int n, void (*initialize) (int*, int));
//heap functions
void crearMonticulo(int * v, int n, pmonticulo m);
int quitarMenor(pmonticulo m);
void ordenarPorMonticulos(int * v, int n);
//tests
void testCrearMonticulo();
void testQuitarMenor();
void testExecutionTime();
void testOrdenarPorMonticulos();
void testComplexities();


void inicializar_semilla() { srand(time(NULL)); }
double microsegundos()
{
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0 ) return 0.;
    return (t.tv_usec + t.tv_sec * 1000000.);
}

void initializeRandom(int * v, int n)
{
    int i, m = 2*n+1;
    for (i = 0; i < n; i++) v[i] = (rand() % m) - n;
}
void initializeAsc(int * v, int n)
{
    int i;
    for (i = 0; i < n; i++)
        v[i] = i + 1;
}
void initializeDesc(int * v, int n)
{
    int i;
    for (i = 0; i < n; i++)
        v[i] = n - i;
}

void printArray(int * v, int n)
{
    int i;

    printf("[");
    for (i = 0; i < n; ++i) printf("%3d", v[i]);
    printf("  ]");
}
void tableTimes(void (*initialize) (int*, int), double n1, double n2, double n3)
{
    int i, j;
    const int n_times = 7, init = 500, limit = init * pow(2, n_times-1), alg_repeats = 3;
    long double t;

    printf("%9s %17s %11s%6.4f %11s%6.4f %11s%6.4f\n", "n", "t(n)", "t/n^", n1, "t/n^", n2, "t/n^", n3);
    for (i = 0; i<alg_repeats; i++)
    {
        for (j = init; j<=limit; j*=2)
        {
            t = getTimesOrder(j, initialize);
            printf("%3s %5d %17.6Lf %17.6Lf %17.6Lf %17.6Lf\n", t < 500? "(*)" : "", j, t, t/pow(j, n1), t/pow(j, n2), t/pow(j, n3));
        }
        printf("\n");
    }    
}

int leftChild(int i) { return 2*i+1; }
int rightChild(int i) { return 2*i+2; }
int size(pmonticulo m) { return m->ultimo + 1; }

double getTimesCreate(int n)
{
    double t, t_init;
    pmonticulo m = MALLOC;
    const int K = 1000, min_t = 500;
    int i, *v = malloc(n * sizeof(int*));

    initializeRandom(v, n);
    t = microsegundos();
    crearMonticulo(v, n, m);
    t = microsegundos() - t;

    if (t < min_t)
    {
        t = microsegundos();
        for (i = 0; i < K; i++)
        {
            initializeRandom(v, n);
            crearMonticulo(v, n, m);
        }
        t = microsegundos() - t;

        t_init = microsegundos();
        for (i = 0; i < K; i++) initializeRandom(v, n);
        t_init = microsegundos() - t_init;

        t = (t - t_init)/K;
    }

    return t;
}
double getTimesOrder(int n, void (*initialize) (int*, int))
{
    double t, t_init;
    const int K = 1000, min_t = 500;
    int i, *v = malloc(n * sizeof(int*));

    initialize(v, n);
    t = microsegundos();
    ordenarPorMonticulos(v, n);
    t = microsegundos() - t;

    if (t < min_t)
    {
        t = microsegundos();
        for (i = 0; i < K; i++)
        {
            initialize(v, n);
            ordenarPorMonticulos(v, n);
        }
        t = microsegundos() - t;

        t_init = microsegundos();
        for (i = 0; i < K; i++) initialize(v, n);
        t_init = microsegundos() - t_init;

        t = (t - t_init)/K;
    }

    return t;    
}

void crearMonticulo(int * v, int n, pmonticulo m) 
{ 
    int i; 
    for (i = 0; i <= (m->ultimo = n - 1); i++) m->vector[i] = v[i]; 
}
int quitarMenor(pmonticulo m)
{
    int i, minpos = 0, min = m->vector[0]; 
    //obtengo el menor
    for (i = 0; i < size(m); i++) if (m->vector[i] < m->vector[minpos]) { minpos = i; min = m->vector[i]; }
    //desde el menor desplazo los elementos borrandolo
    for (i = minpos; i < m->ultimo; i++) m->vector[i] = m->vector[i+1];
    //reduzco el tamaño en 1
    m->ultimo--;
    return min;
}
void ordenarPorMonticulos(int * v, int n)
{
    pmonticulo m = MALLOC;
    crearMonticulo(v, n, m);
    int i;

    for (i = 0; i < n; i++) v[i] = quitarMenor(m);
}

void testCrearMonticulo()
{
    pmonticulo m = MALLOC;
    int n = 10, v[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };

    crearMonticulo(v, n, m);

    printf("------------------------\n  TEST CREAR MONTICULO\n------------------------\nVector: ");
    printArray(v, n);
    printf("\nVector del heap: ");
    printArray(m->vector, size(m));
    printf("\nUltima posicion: %d\n\n", m->ultimo);
}
void testQuitarMenor()
{
    pmonticulo m = MALLOC;
    int n = 10, *v = MALLOC;
    initializeRandom(v, n);
    crearMonticulo(v, n, m);

    printf("---------------------\n  TEST QUITAR MENOR\n---------------------\nVector inicial aleatorio: ");
    printArray(m->vector, size(m));
    printf("\nElemento devuelto: %d\nVector final: ", quitarMenor(m));
    printArray(m->vector, size(m));
    printf("\n\n");
}
void testExecutionTime()
{
    int i, j;
    const int n_times = 8, init = 500, limit = init * pow(2, n_times-1), alg_repeats = 3;
    long double t;

    printf("-----------------------------------\n  TEST COMPLEJIDAD CREARMONTICULO\n-----------------------------------\n%9s %17s %17s\n", "n", "t(n)", "t/n");
    for (i = 0; i<alg_repeats; i++)
    {
        for (j = init; j<=limit; j*=2)
        {
            t = getTimesCreate(j);
            printf("%3s %5d %17.6Lf %17.6Lf\n", t < 500? "(*)" : "", j, t, t/j);
        }
        printf("\n");
    }

    printf("Se puede observar que t/n tiende a una constante por lo que este proceso tiene complejidad O(n)\n\n");
}
void testOrdenarPorMonticulos()
{
    int * v = MALLOC, n = 10;
    printf("---------------------------------\n  TEST ORDENACION POR MONTICULOS\n---------------------------------\nAleatorio:\n");
    initializeRandom(v, n);
    printArray(v, n);
    printf(" -> ");
    ordenarPorMonticulos(v, n);
    printArray(v, n);
    printf("\nDescendente:\n");
    initializeDesc(v, n);
    printArray(v, n);
    printf(" -> ");
    ordenarPorMonticulos(v, n);
    printArray(v, n);
    printf("\nAscendente:\n");
    initializeAsc(v, n);
    printArray(v, n);
    printf(" -> ");
    ordenarPorMonticulos(v, n);
    printArray(v, n);
    printf("\n\n");
}
void testComplexities()
{
    printf("----------------------\n  TEST COMPLEJIDADES\n----------------------\nAleatorio:\n\n");
    tableTimes(initializeRandom, 1.8, 2, 2.2);
    printf("Descendente:\n\n");
    tableTimes(initializeRandom, 1.8, 2, 2.2);
    printf("Ascendente:\n\n");
    tableTimes(initializeRandom, 1.8, 2, 2.2);
}


int main()
{
    testCrearMonticulo();
    testQuitarMenor();
    testExecutionTime();
    testOrdenarPorMonticulos();
    testComplexities();

    return 0;
}