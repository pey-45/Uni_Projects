#include "aux_functions.h"


int TrocearCadena(char * cadena, char * trozos[])
{
    /*asigna al array de cadenas los elementos separados 
    por espacios, tabs o saltos de línea y devuelve la cantidad de trozos*/
    int i;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL)
        return 0;
    for (i = 1; (trozos[i]=strtok(NULL," \n\t"))!=NULL; ++i);
    return i;
}

tPosL getPosByDF(int df, tList L)
{
    char *string = malloc(MAX_PROMPT*sizeof(char*)), **strings = malloc(MAX_PROMPT*sizeof(char*));
    tPosL i;

    /*recorre la lista de archivos abiertos y devuelve la 
    posición del elemento con el df dado y nulo si no lo encuentra*/
    for (i = first(L); i!=LNULL; i = next(i))
    {
        strcpy(string, getItem(i));
        TrocearCadena(string, strings);
        if (atoi(strings[1])==df)
            break;
    }

    free(string);
    free(strings);

    return i;
}

void printOpenListByDF(tList L)
{
    int index = 0, last_index;
    char *string = malloc(MAX_PROMPT*sizeof(char*)), **strings = malloc(MAX_PROMPT*sizeof(char*));
    tPosL i;

    //almacena en last_index el df de la ultima posicion de archivos abiertos
    strcpy(string, getItem(last(L)));
    TrocearCadena(string, strings);
    last_index = atoi(strings[1]);

    //recorre la lista hasta el último elemento imprimiendo los elementos que encuentra
    while(index<=last_index)
    {
        i = getPosByDF(index, L);
        if (i!=NULL)
        {
            printItem(i);
            printf("\n");
        }
        ++index;
    }

    free(string);
    free(strings);
}

void printOpenListByDFUntil(int limit, tList L)
{
    int index = 0;
    tPosL i;

    //recorre la lista hasta el límite imprimiendo los elementos que encuentra
    while(index<limit)
    {
        i = getPosByDF(index, L);
        if (i!=NULL)
        {
            printItem(i);
            printf("\n");
        }
        ++index;
    }
}

void printHistUntil(int limit, tList L)
{
    tPosL i;
    int index = 0;

    //imprime los elementos del historial en un determinado formato
    for (i = first(L); i!=LNULL && index<limit; i = next(i))
    {
        printf("%d->", index);
        printItem(i);
        printf("\n");
        ++index;
    }
}

bool isDigitString(char * string)
{
    //devuelve si el string consta únicamente de digitos
    for (int i = 0; i < strlen(string); ++i)
    {
        if (!isdigit(string[i]))
            return false;
    }
    return true;
}