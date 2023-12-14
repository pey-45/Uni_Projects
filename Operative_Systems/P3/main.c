#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

int main() {
    
    char *string = malloc(2048);

    strcpy(string, "hello");
    strcpy(string, "worldxd");

    free(string);

    return 0;
}
