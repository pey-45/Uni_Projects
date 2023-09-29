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

//Declaración de funciones
void inicializar_semilla();
void aleatorio(int * v, int n);
void printArray(int * v, int n);
double microsegundos();

int sumaSubMax1(int * v, int n);
int sumaSubMax2(int * v, int n);
void test1();
void test2();
double getTimes(int n, int (*sumaSubMax)(int*, int));
void tableSumaSubMax(int (*sumaSubMax)(int*, int), int n_function, double n1, double n2, double n3);
void test3();


void inicializar_semilla()
{
    srand(time(NULL));
}
void aleatorio(int * v, int n)
{
    int i, m = 2*n+1;

    for (i = 0; i < n; ++i)
        v[i] = (rand() % m) - n;
}
void printArray(int * v, int n)
{
    int i;

    printf("[");
    for (i = 0; i < n; ++i)
        printf("%3d", v[i]);
    printf(" ]");
}
double microsegundos()
{
    struct timeval t;

    if (gettimeofday(&t, NULL) < 0 )
        return 0.;
    return (t.tv_usec + t.tv_sec * 1000000.);
}

int sumaSubMax1(int * v, int n)
{
    int sumaMax = 0, estaSuma, i, j;

    for (i = 0; i < n; i++)
    {
        estaSuma = 0;
        for (j = i; j < n; j++)
        {
            estaSuma += v[j];
            if (estaSuma > sumaMax)
                sumaMax = estaSuma;
        }
    }

    return sumaMax;
}

int sumaSubMax2(int * v, int n)
{
    int estaSuma = 0, sumaMax = 0, i;

    for (i = 0; i < n; ++i)
    {
        estaSuma += v[i];
        if (estaSuma>sumaMax)
            sumaMax = estaSuma;
        else if (estaSuma < 0)
            estaSuma = 0;
    }

    return sumaMax;
}

void test1()
{
    const int rows = 6, cols = 5;
    int i;
    int matrix[6][5] =  {
            { -9, 2, -5, -4, 6 },
            { 4, 0, 9, 2, 5 },
            { -2, -1, -9, -7, -1 },
            { 9, -2, 1, -7, -8 },
            { 15, -2, -5, -4, 16 },
            { 7, -5, 6, 7, -7 }
    };

    printf("\n%33s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i = 0; i<rows; ++i)
    {
        printArray(matrix[i], cols);
        printf("\t%5s%15d%15d\n", "", sumaSubMax1(matrix[i], cols), sumaSubMax2(matrix[i], cols));
    }
}

void test2()
{
    const int size = 9, n_vectors = 8;
    int i, *v = malloc(size * sizeof(int*));

    printf("\n\t%25s%15s%15s\n", "", "sumaSubMax1", "sumaSubMax2");
    for (i = 0; i<n_vectors; ++i)
    {
        aleatorio(v, size);
        printArray(v, size);
        printf("\t\t%4d%15d\n", sumaSubMax1(v, size), sumaSubMax2(v, size));
    }

    free(v);
}

double getTimes(int n, int (*sumaSubMax)(int*, int))
{
    double ta, t, t1, t2;
    const int K = 1000, min_t = 500;
    int i, *v = malloc(n * sizeof(int*));

    aleatorio(v, n);
    ta = microsegundos();
    sumaSubMax(v, n);
    t = microsegundos() - ta;

    if (t < min_t)
    {
        ta = microsegundos();
        for (i = 0; i < K; i++)
        {
            aleatorio(v, n);
            sumaSubMax(v, n);
        }
        t1 = microsegundos() - ta;

        ta = microsegundos();
        for (i = 0; i < K; i++)
            aleatorio(v, n);
        t2 = microsegundos() - ta;

        t = (t1 - t2) / K;
    }

    free(v);
    return t;
}

void tableSumaSubMax(int (*sumaSubMax)(int*, int), const int n_function, double n1, double n2, double n3)
{
    int i, j;
    const int n_times = 8, init = 500, limit = init * pow(2, n_times-1), alg_repeats = 4;
    long double t;

    printf("\nsumaSubMax%d:\n\n", n_function);
    printf("%9s %17s %11s%6.4f %11s%6.4f %11s%6.4f\n", "n", "t(n)", "t/n^", n1, "t/n^", n2, "t/n^", n3);
    for (i = 0; i<alg_repeats; i++)
    {
        for (j = init; j<=limit; j*=2)
        {
            t = getTimes(j, sumaSubMax);
            printf("%3s %5d %17.6Lf %17.6Lf %17.6Lf %17.6Lf\n", t < 500? "(*)" : "", j, t, t/pow(j, n1), t/pow(j, n2), t/pow(j, n3));
        }
        printf("\n");
    }
}

void test3()
{
    tableSumaSubMax(sumaSubMax1, 1, 1.8, 1.9975, 2.2);
    tableSumaSubMax(sumaSubMax2, 2, 0.8, 0.9375, 1.2);
}


int main()
{
    test1();
    printf("\n\n");
    test2();
    printf("\n\n");
    test3();

    return 0;
}