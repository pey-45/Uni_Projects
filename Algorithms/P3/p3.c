//Algoritmos P3
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
//prints
void printArray(int * v, int n);
void tableTimes(void (*initialize) (int*, int), double n1, double n2, double n3);
//heap info
bool esMonticuloVacio(pmonticulo m);
int leftChild(int i);
int rightChild(int i);
int size(pmonticulo m);
//times
double getTimesCreate(int n);
double getTimesOrder(int n, void (*initialize) (int*, int));
//heap functions
void swap(int * a, int * b);
void hundir(pmonticulo m, int i);
void crearMonticulo(int * v, int n, pmonticulo m);
int quitarMenor(pmonticulo m);
void ordenarPorMonticulos(int * v, int n);
//tests
void testCrearMonticulo();
void testQuitarMenor();
void testExecutionTime();
void testOrdenarPorMonticulos();
void testComplexities();

//rand
void inicializar_semilla() { srand(time(NULL)); }
double microsegundos()
{
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0 ) return 0.;
    return (t.tv_usec + t.tv_sec * 1000000.);
}

//initializations
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

//prints
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

//heap info
bool esMonticuloVacio(pmonticulo m) { return m->ultimo==-1; }
int leftChild(int i) { return 2*i+1; }
int rightChild(int i) { return 2*i+2; }
int size(pmonticulo m) { return m->ultimo + 1; }

//times
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

//heap functions
void swap(int * a, int * b)
{
    int aux = *a;
    *a = *b;
    *b = aux;
}
void hundir(pmonticulo m, int i)
{
    int smallest;
    do
    {
        smallest = i;
        //si alguno de los hijos es menor
        if (rightChild(i) <= m->ultimo && m->vector[rightChild(i)] < m->vector[i]) smallest = rightChild(i);
        if (leftChild(i) <= m->ultimo && m->vector[leftChild(i)] < m->vector[i]) smallest = leftChild(i);

        //se activara este if
        if (smallest!=i)
        {
            /*se intercambia el hijo con el padre y el que queda abajo se hunde con sus 
            respectivos hijos para evitar superposiciones incorrectas*/
            swap(&m->vector[i], &m->vector[smallest]);
            //realmente se hunde el mayor, pero se intercambiaron las posiciones
            hundir(m, smallest);
        }
    }
    //mientras que el trío de nodos no sea válido (padre > alguno de los hijos)
    while (smallest != i);
}
void crearMonticulo(int * v, int n, pmonticulo m)
{
    int i;
    //se copia el vector
    for (i = 0; i < n; i++) m->vector[i] = v[i];
    m->ultimo = n-1;

    //y se hunden las posiciones desde el ultimo padre (ultimo elemento no hoja) hasta el padre absoluto
    for (i = m->ultimo / 2; i >= 0; i--) hundir(m, i);
}
int quitarMenor(pmonticulo m)
{
    int x;

    if (esMonticuloVacio(m)) return -1;
    
    /*se guarda el menor número, se sobreescribe poniendo en la 
    primera posición el que era el último y se reduce el tamaño*/
    x = m->vector[0];
    m->vector[0] = m->vector[m->ultimo];
    m->ultimo--;

    //como el primer elemento no es válido como padre (estaba en la última posición), se hunde
    if (m->ultimo > 0) hundir(m, 0);

    //se devuelve el que era el menor
    return x;
}
void ordenarPorMonticulos(int * v, int n)
{
    //se crea un montículo en base al vector
    pmonticulo m = MALLOC;
    crearMonticulo(v, n, m);
    int i;

    //y se va quitando del monticulo el menor elemento en cada iteración y se añade al vector
    for (i = 0; i < n; i++) v[i] = quitarMenor(m);
}

//tests
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

    printf("---------------------\n  TEST QUITAR MENOR\n---------------------\nVector inicial del montículo: ");
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
    tableTimes(initializeDesc, 1.8, 2, 2.2);
    printf("Ascendente:\n\n");
    tableTimes(initializeAsc, 1.8, 2, 2.2);
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