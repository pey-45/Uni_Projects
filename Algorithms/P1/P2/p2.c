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


void inicializar_semilla();
void aleatorio(int * v, int n);
void initializeAsc(int * v, int n);
void initializeDesc(int * v, int n);
void printArray(int * v, int n);
double microsegundos();

void ord_ins (int * v, int n);
void ord_shell (int * v, int n);
void test(char * inicializacion, void (*initialize) (int*, int), void (*ordenacion) (int*, int));
double getTimes(int n, void (*order)(int*, int), void (*initialize) (int*, int));
void tableTimes(void (*order)(int*, int), void (*initialize) (int*, int), char * tipo_ord, double n1, double n2, double theoric, double n3);

void executeTests();
void executeTables();


void inicializar_semilla()
{
    srand(time(NULL));
}
void aleatorio(int * v, int n)
{
    int i, m = 2*n+1;

    for (i = 0; i < n; i++)
        v[i] = (rand() % m) - n;
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
    for (i = 0; i < n; ++i)
        printf("%4d", v[i]);
    printf("  ]");
}
double microsegundos()
{
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0 )
        return 0.;
    return (t.tv_usec + t.tv_sec * 1000000.);
}


void ord_ins (int * v, int n)
{
    int i, j, x;

    for (i = 1; i < n; i++)
    {
        x = v[i];
        j = i - 1;

        while(j >= 0 && v[j] > x)
        {
            v[j+1] = v[j];
            j--;
        }

        v[j+1] = x;
    }
}

void ord_shell (int * v, int n)
{
    int i, j, tmp, incremento = n;
    bool seguir;

    do
    {
        incremento /= 2;

        for (i = incremento + 1; i < n; i++)
        {
            tmp = v[i];
            j = i;
            seguir = true;

            while (j-incremento >= 0 && seguir)
            {
                if (tmp < v[j-incremento])
                {
                    v[j] = v[j-incremento];
                    j -= incremento;
                }
                else seguir = false;
            }

            v[j] = tmp;
        }
    } 
    while (incremento != 1);
}

void test(char * inicializacion, void (*initialize) (int*, int), void (*ordenacion) (int*, int))
{
    const int n = 10;
    int *v = malloc(n * sizeof(int*));

    printf("%s\n", inicializacion);
    initialize(v, n);
    printArray(v, n);
    ordenacion(v, n);
    printf(" --> ");
    printArray(v, n);
    printf("\n");

    free(v);
}

double getTimes(int n, void (*order)(int*, int), void (*initialize) (int*, int))
{
    double ta, t, t1, t2;
    const int K = 1000, min_t = 500;
    int i, *v = malloc(n * sizeof(int*));

    initialize(v, n);
    ta = microsegundos();
    order(v, n);
    t = microsegundos() - ta;

    if (t < min_t)
    {
        ta = microsegundos();
        for (i = 0; i < K; i++)
        {
            initialize(v, n);
            order(v, n);
        }
        t1 = microsegundos() - ta;

        ta = microsegundos();
        for (i = 0; i < K; i++)
            initialize(v, n);
        t2 = microsegundos() - ta;

        t = (t1 - t2) / K;
    }

    free(v);
    return t;
}

void tableTimes(void (*order)(int*, int), void (*initialize) (int*, int), char * tipo_ord, double n1, double n2, double theoric, double n3)
{
    int i, j;
    const int n_times = 7, init = 500, limit = init * pow(2, n_times-1), alg_repeats = 3;
    long double t;

    printf("\n%s:\n\n", tipo_ord);
    printf("%9s %17s %11s%6.4f %11s%6.4f %11s%6.4f %11s%6.4f\n", "n", "t(n)", "t/n^", n1, "t/n^", n2, "t/n^", theoric, "t/n^", n3);
    for (i = 0; i<alg_repeats; i++)
    {
        for (j = init; j<=limit; j*=2)
        {
            t = getTimes(j, order, initialize);
            printf("%3s %5d %17.6Lf %17.6Lf %17.6Lf %17.6Lf %17.6Lf\n", t < 500? "(*)" : "", j, t, t/pow(j, n1), t/pow(j, n2), t/pow(j, theoric), t/pow(j, n3));
        }
        printf("\n");
    }
}

void executeTests()
{
    test("Ascendente:", initializeAsc, ord_ins);
    test("Descendente:", initializeDesc, ord_ins);
    test("Aleatorio:", aleatorio, ord_ins);
    printf("\n");

    printf("Ordenacion shell:\n");
    test("Ascendente:", initializeAsc, ord_shell);
    test("Descendente:", initializeDesc, ord_shell);
    test("Aleatorio:", aleatorio, ord_shell);
    printf("\n\n");
}

void executeTables()
{
    tableTimes(ord_ins, initializeAsc, "Insercion ascendente", .8, 1, 1, 1.2);
    tableTimes(ord_ins, aleatorio, "Insercion desordenado", 1.9, 2, 2, 2.1);
    tableTimes(ord_ins, initializeDesc, "Insercion decendente", 1.8, 2, 2, 2.2);
    tableTimes(ord_shell, initializeAsc, "Shell ascendente", .8, 1.13, 1, 1.5);
    tableTimes(ord_shell, aleatorio, "Shell desordenado", .8, 1.19, 1, 1.6);
    tableTimes(ord_shell, initializeDesc, "Shell descendente", .8, 1.15, 1, 1.5);
}

int main()
{
    executeTests();
    executeTables();

    return 0;
}