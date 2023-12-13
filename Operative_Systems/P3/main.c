#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main() {
    srand(time(NULL)); // Inicializar la semilla para generar números aleatorios
    int i = 0;

    while (i < 20) {
        int numeroAleatorio = rand();
        printf("Número aleatorio: %d\n", numeroAleatorio);
        sleep(1); // Espera un segundo
        i++;
    }

    return 0;
}
