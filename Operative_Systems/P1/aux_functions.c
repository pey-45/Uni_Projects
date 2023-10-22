#include "aux_functions.h"

//liberar strings
void freeStrings(int n, ...)
{
    va_list args;
    va_start(args, n);
    int i;

    for (i = 0; i < n; i++)
    {
        char *aux = va_arg(args, char*);
        if (aux!=NULL)
        {
            free(aux);
            aux = NULL;
        }
    }

    va_end(args);
}

//liberar arrays de strings cuyos elementos no apuntan a ninguna cadena anteriormente liberada
//en otras palabras, para los cuales no tienen elementos o han sido procesados por TrocearCadena
void freeMatrixAllElements(int n, ...)
{
    va_list args;
    va_start(args, n);
    int i, j;

    for (i = 0; i < n; i++)
    {
        char **aux = va_arg(args, char**);
        if (aux!=NULL)
        {
            for (j = 0; aux[j] != NULL; j++) 
            {
                free(aux[j]);
                aux[j] = NULL;
            }
            free(aux);
            aux = NULL;
        }
    }

    va_end(args);
}

//libera arrays de strings
void freeMatrix(int n, ...)
{
    va_list args;
    va_start(args, n);
    int i;

    for (i = 0; i < n; i++)
    {
        char **aux = va_arg(args, char**);
        if (aux!=NULL)
        {
            free(aux);
            aux = NULL;
        }
    }

    va_end(args);
}

int TrocearCadena(char * cadena, char ** trozos)
{
    /*asigna al array de cadenas los elementos separados 
    por espacios, tabs o saltos de línea y devuelve la cantidad de trozos*/
    int i;
    if ((trozos[0]=strtok(cadena," \n\t"))==NULL) return 0;
    for (i = 1; (trozos[i]=strtok(NULL," \n\t"))!=NULL; ++i);
    return i;
}

tPosL getPosByDF(int df, tList L)
{
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (string == NULL || strings == NULL)
    {
        printf("Error al asignar memoria.");
        freeStrings(1, string);
        freeMatrix(1, strings);
        return NULL;
    }
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

    freeStrings(1, string);
    freeMatrix(1, strings);

    return i;
}

void printOpenListByDF(tList L)
{
    int index = 0, last_index;
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (string == NULL || strings == NULL)
    {
        printf("Error al asignar memoria.");
        freeStrings(1, string);
        freeMatrix(1, strings);
        return;
    }
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

    freeStrings(1, string);
    freeMatrix(1, strings);
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
    for (int i = 0; i < (int)strlen(string); ++i)
    {
        if (!isdigit(string[i]))
            return false;
    }
    return true;
}

void printCurrentDir()
{
    char *dir = MALLOC;
    if (dir == NULL)
    {
        printf("Error al asignar memoria.");
        return;
    }

    getcwd(dir, MAX_PROMPT);
    printf("%s\n", dir);

    freeStrings(1, dir);
}

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { //and bit a bit con los bits de formato,0170000
        case S_IFSOCK: return 's'; //socket
        case S_IFLNK: return 'l'; //symbolic lin
        case S_IFREG: return '-'; //fichero normal
        case S_IFBLK: return 'b'; //block device
        case S_IFDIR: return 'd'; //directorio
        case S_IFCHR: return 'c'; //char device
        case S_IFIFO: return 'p'; //pipe
        default: return '?'; //desconocido, no deberia aparecer
     }
}

void printStat(char * file, struct stat attr, char * print_mode, bool link, bool is_from_list)
{
    int year, month, day, hour, min;
    nlink_t n_link;
    ino_t n_ino;
    char *permissions = malloc(12*sizeof(char*)); //no hace falta mas
    if (permissions == NULL)
    {
        printf("Error al asignar memoria.");
        return;
    }
    off_t size;
    struct passwd *prop;
    struct group *group;
    char *link_path = MALLOC;
    if (link_path == NULL)
    {
        printf("Error al asignar memoria.");
        return;
    }
    ssize_t length = readlink(file, link_path, sizeof(link_path) - 1);
    if (length != -1) link_path[length] = '\0'; 

    if (!lstat(file, &attr))
    {
        time_t timestamp = attr.st_mtime;
        struct tm *timeinfo = localtime(&timestamp);
        year = timeinfo->tm_year;
        month = timeinfo->tm_mon;
        day = timeinfo->tm_mday;
        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
        n_link = attr.st_nlink;
        n_ino = attr.st_ino;
        size = attr.st_size;
        prop = getpwuid(attr.st_uid);
        group = getgrgid(attr.st_gid);
    }
    else
    {
        perror("No se ha podido hacer lstat");
        return;
    }

    //si hay long se imprime long, si hay acc y long se imprime long, si no hay parametros se imprime solo el tamaño y el nombre.
    //el printf del final esta hecho para que siempre se imprima el tamaño y el nombre pero el link solo cuando toca
    if (!strcmp(print_mode, "long")) printf("%d/%02d/%02d-%02d:%02d   %lu (%lu)    %s    %s %s", 1900+year, month+1, day, hour, min, n_link, n_ino, prop->pw_name, group->gr_name, permissions);
    else if (!strcmp(print_mode, "acc")) printf("%d/%02d/%02d-%02d:%02d", 1900+year, month+1, day, hour, min);

    printf("    %d %s    %s", (int)size, is_from_list? getLastNamePath(file):file, link? link_path:"");
}

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    //propietario
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    //grupo
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    //resto
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    //setuid, setgid y stickybit
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}


bool includesString(char * string, char ** strings)
{
    if (strings == NULL || string == NULL) return false;
    int i;
    for (i = 0; strings[i]!=NULL; i++) if (!strcmp(string, strings[i])) return true;
    return false;
}

//no podemos incluir la libreria functions.h dado que se crearian redefiniciones indeseadas
//se pasa true a las funciones ya que esta funcion está hecha para f_list
void aux_stat(char ** command)
{
    struct stat attr;
    int i;
    //reservo memoria para las listas de archivos y argumentos
    char *file = MALLOC, **args = MALLOC_PTR;
    if (file == NULL || args == NULL)
    {
        printf("Error al asignar memoria.");
        freeMatrix(1, args);
        freeStrings(1, file);
        return;
    }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++) args[i] = NULL;

    bool in_files = 0;
    int breakpoint = 0;

    for (i = 1; command[i]!=NULL; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long")!=0 && strcmp(command[i], "-link")!=0 && strcmp(command[i], "-acc")!=0) in_files = 1;

        if (!in_files)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[breakpoint] = MALLOC;
            if (args[breakpoint]==NULL)
            {
                perror("Error al asignar memoria.");
                freeMatrixAllElements(1, args);
                freeStrings(1, file);
                return;
            }
            args[breakpoint][0] = '\0'; //para evitar fallos con el strcmp

            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[breakpoint++], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[breakpoint++], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[breakpoint++], "acc");
            else
            {
                //si se repite un parámetro se libera la posicion ya que no se añadirá nada, tampoco incrementa breakpoint
                freeStrings(1, args[breakpoint]);
                args[breakpoint] = NULL;
            }
        }       
        else file = command[i];
    }

    if (includesString("long", args))
    {
        if (stat(file, &attr) == 0) 
        {
            printf(" ");
            printStat(file, attr, "long", includesString("link", args), true);
                
        }
    }
    else if (includesString("acc", args))
    {
        if (stat(file, &attr) == 0) 
        {
            printf(" ");
            printStat(file, attr, "acc", includesString("link", args), true);
        }
    }
    else if (includesString("link", args))
    {
        if (stat(file, &attr) == 0)
        {
            printStat(file, attr, "link", true, true);
            printf("\n");
        }
    }
    else
    {
        if (stat(file, &attr) == 0) 
        {
            printStat(file, attr, "few", includesString("acc", args), true);
            printf("\n");
        }
    }

    freeMatrixAllElements(1, args);
    freeStrings(1, file);
}

void printAsStat(char * dir, char ** args)
{
    char *command = MALLOC, **full_command = MALLOC_PTR;
    if (command == NULL || full_command == NULL)
    {
        printf("Error al asignar memoria.");
        freeStrings(1, command);
        freeMatrix(1, full_command);
        return;
    }

    strcpy(command, "stat ");

    if (includesString("long", args)) strcat(command, "-long ");
    if (includesString("acc", args)) strcat(command, "-acc ");
    if (includesString("link", args)) strcat(command, "-link ");

    strcat(command, dir);

    TrocearCadena(command, full_command);

    aux_stat(full_command);

    freeStrings(1, command);
    freeMatrix(1, full_command);
}

void printDirElements(char * _dir, char ** args, char *mode, bool hid) 
{
    char *subroute = MALLOC;
    struct dirent *entry;
    //se crea una variable tipo directorio que almacena las caracteristicas del directorio de entrada
    DIR *dir = opendir(_dir);

    if (dir == NULL) 
    {
        perror("Error al abrir el directorio.");
        return;
    }

    else if (!strcmp(mode, "before"))
    {
        while ((entry = readdir(dir)) != NULL) if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            printDirElementsRB(subroute, args, hid);
        }

        rewinddir(dir);
        printf("************%s\n", _dir);
        //luego listo todos los elementos del directorio
        while ((entry = readdir(dir)) != NULL) if (entry->d_name[0]!='.' || hid) 
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            printAsStat(subroute, args);
        }
    }
    else if (!strcmp(mode, "after"))
    {
        printf("************%s\n", _dir);
        //primero listo todos los elementos del directorio
        while ((entry = readdir(dir)) != NULL) if (entry->d_name[0]!='.' || hid)
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            printAsStat(subroute, args);
        } 

        //vuelvo a recorrer el directorio y si el elemento es un directorio se imprimen sus elementos de la misma forma
        rewinddir(dir);
        while ((entry = readdir(dir)) != NULL) if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            printDirElementsRA(subroute, args, hid);
        }
    }
    else
    {
        printf("************%s\n", _dir);
        //va imprimiendo uno por uno los elementos del directorio, entrando a un subdirectorio y recorriendo este si asi se necesitara
        while ((entry = readdir(dir)) != NULL) if (entry->d_name[0]!='.' || hid) printAsStat(entry->d_name, args);
    }

    closedir(dir);
}

//se imprimen 
void printDirElementsRB(char * _dir, char ** args, bool hid) 
{
    char *subroute = MALLOC;
    struct dirent *entry;
    //se crea una variable tipo directorio que almacena las caracteristicas del directorio de entrada 
    DIR *dir = opendir(_dir);

    if (dir == NULL) 
    {
        perror("Error al abrir el directorio.");
        return;
    }

    //primero recorro el directorio y si el elemento es un directorio se imprimen sus elementos de la misma forma
    while ((entry = readdir(dir)) != NULL) if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
    {
        snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
        printDirElementsRB(subroute, args, hid);
    }

    rewinddir(dir);
    printf("************%s\n", _dir);
    //luego listo todos los elementos del directorio
    while ((entry = readdir(dir)) != NULL) if (entry->d_name[0]!='.' || hid) 
    {
        snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
        printAsStat(subroute, args);
    }

    freeStrings(1, subroute);
    closedir(dir);
}

void printDirElementsRA(char * _dir, char ** args, bool hid) 
{
    char *subroute = MALLOC;
    struct dirent *entry;
    //se crea una variable tipo directorio que almacena las caracteristicas del directorio de entrada 
    DIR *dir = opendir(_dir);

    if (dir == NULL) 
    {
        perror("Error al abrir el directorio.");
        return;
    }

    printf("************%s\n", _dir);
    //primero listo todos los elementos del directorio
    while ((entry = readdir(dir)) != NULL) if (entry->d_name[0]!='.' || hid)
    {
        snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
        printAsStat(subroute, args);
    } 

    //vuelvo a recorrer el directorio y si el elemento es un directorio se imprimen sus elementos de la misma forma
    rewinddir(dir);
    while ((entry = readdir(dir)) != NULL) if (entry->d_type == DT_DIR && strcmp(entry->d_name, ".")!=0 && strcmp(entry->d_name, "..")!=0)
    {
        snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
        printDirElementsRA(subroute, args, hid);
    }

    freeStrings(1, subroute);
    closedir(dir);
}

char *getLastNamePath(char *dir)
{
    char *last_slash = strrchr(dir, '/');
    return last_slash != NULL? last_slash + 1:dir;
}
