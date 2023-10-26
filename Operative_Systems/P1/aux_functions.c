#include "aux_functions.h"

//libera strings y cadenas de strings con solo elementos nulos
//sirve para trocearcadena
//IMPORTANTE LIBERAR PRIMERO EL STRING Y LUEGO LA CADENA DE PUNTEROS A STRING
void freeAll(int n, ...)
{
    va_list args;
    va_start(args, n);
    int i;

    for (i = 0; i < n; i++)
    {
        char *aux = va_arg(args, char*);
        if (aux) { free(aux); aux = NULL; }
    }

    va_end(args);
}

//libera cadenas de strings liberando uno por uno todos sus elementos
void freeAllRec(int n, ...)
{
    va_list args;
    va_start(args, n);
    int i, j;

    for (i = 0; i < n; i++)
    {
        char **aux = va_arg(args, char**);
        if (aux)
        {
            for (j = 0; aux[j]; j++) { free(aux[j]); aux[j] = NULL; }
            free(aux); aux = NULL;
        }
    }

    va_end(args);
}

int TrocearCadena(char * cadena, char ** trozos)
{
    /*asigna al array de cadenas los elementos separados 
    por espacios, tabs o saltos de línea y devuelve la cantidad de trozos*/
    int i;

    if (!(trozos[0]=strtok(cadena," \n\t"))) return 0;
    for (i = 1; (trozos[i]=strtok(NULL," \n\t")); ++i);
    return i;
}

tPosL getPosByDF(int df, tList L)
{
    tPosL i;
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (!string || !strings) { perror("Error al asignar memoria"); freeAll(2, string, strings); return NULL; }

    /*recorre la lista de archivos abiertos y devuelve la 
    posición del elemento con el df dado y nulo si no lo encuentra*/
    for (i = first(L); i; i = next(i))
    {
        strcpy(string, getItem(i));
        TrocearCadena(string, strings);
        if (atoi(strings[1])==df) break;
    }

    freeAll(2, string, strings);

    return i;
}

tPosL getLastPosByDF(tList L)
{
    tPosL i;
    char *string = MALLOC, **strings = MALLOC_PTR;
    int max = 0;
    if (!string || !strings) { perror("Error al asignar memoria"); freeAll(2, string, strings); return NULL; }

    for (i = first(L); i; i = next(i))
    {
        strcpy(string, getItem(i));
        TrocearCadena(string, strings);
        if (atoi(strings[1]) > max) max = atoi(strings[1]);
    }

    freeAll(2, string, strings);

    return getPosByDF(max, L);
}

void printOpenListByDF(tList L)
{
    int i, last_index;
    tPosL pos;
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (!string || !strings) { perror("Error al asignar memoria"); freeAll(2, string, strings); return; }

    //almacena en last_index el df de la ultima posicion de archivos abiertos
    strcpy(string, getItem(getLastPosByDF(L)));
    TrocearCadena(string, strings);
    last_index = atoi(strings[1]);

    //recorre la lista hasta el último elemento imprimiendo los elementos que encuentra
    for (i = 0; i <= last_index; i++)
    {
        pos = getPosByDF(i, L);
        if (pos) { printItem(pos); printf("\n"); }
    }

    freeAll(2, string, strings);
}

void printOpenListByDFUntil(int limit, tList L)
{
    int i;
    tPosL pos;

    //recorre la lista hasta el límite imprimiendo los elementos que encuentra
    for (i = 0; i < limit; i++)
    {
        pos = getPosByDF(i, L);
        if (pos){ printItem(pos); printf("\n"); }
    }
}

void printHistUntil(int limit, tList L)
{
    tPosL pos;
    int i = 0;

    //imprime los elementos del historial en un determinado formato
    for (pos = first(L); pos && i++<limit; pos = next(pos)) { printf("%d->", i); printItem(pos); printf("\n"); }
}

bool isDigitString(char * string)
{
    if (!string) return false;
    //devuelve si el string consta únicamente de digitos
    for (int i = 0; i < (int)strlen(string); i++) if (!isdigit(string[i])) return false;
    return true;
}

void printCurrentDir()
{
    char *dir = MALLOC;
    if (!dir) { perror("Error al asignar memoria"); return; }

    getcwd(dir, MAX_PROMPT);
    printf("%s\n", dir);

    freeAll(1, dir);
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

//is_from_list determina si queremos imprimir los archivos con la ruta entera o solo el nombre
void printStat(char * file, struct stat attr, char * print_mode, bool link, bool is_from_list)
{
    int year, month, day, hour, min;
    size_t len;
    nlink_t n_link;
    mode_t mode;
    ino_t n_ino;
    char *permissions = malloc(12*sizeof(char*)), *link_path = MALLOC;
    if (!permissions || !link_path) { perror("Error al asignar memoria"); freeAll(2, permissions, link_path); return; }
    off_t size;
    struct passwd *prop;
    struct group *group;
    initializeString(link_path);
    len = readlink(file, link_path, MAX_PROMPT);
    if (len!=-1) link_path[len] = '\0';    

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
        mode = attr.st_mode;
        n_ino = attr.st_ino;
        size = attr.st_size;
        prop = getpwuid(attr.st_uid);
        group = getgrgid(attr.st_gid);
    }
    else { fprintf(stderr, "Error al acceder a %s: %s\n", file, strerror(errno)); freeAll(2, permissions, link_path); return; }

    ConvierteModo(mode, permissions);

    //si hay long se imprime long, si hay acc y long se imprime long, si no hay parametros se imprime solo el tamaño y el nombre.
    //el printf del final esta hecho para que siempre se imprima el tamaño y el nombre pero el link solo cuando toca
    if (!strcmp(print_mode, "long")) printf("%d/%02d/%02d-%02d:%02d   %lu (%lu)    %s    %s %s", 1900+year, month+1, day, hour, 
                                             min, n_link, n_ino, prop->pw_name, group->gr_name, permissions);
    else if (!strcmp(print_mode, "acc")) printf("%d/%02d/%02d-%02d:%02d", 1900+year, month+1, day, hour, min);

    printf("%8d  %s         %s\n", (int)size, is_from_list? getLastNamePath(file):file, link? link_path:"");

    freeAll(2, permissions, link_path);
}

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy(permisos,"---------- ");
    
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
    if (!strings || !string) return false;
    int i;
    for (i = 0; strings[i]; i++) if (!strcmp(string, strings[i])) return true;
    return false;
}

//no podemos incluir la libreria functions.h dado que se crearian redefiniciones indeseadas
//se pasa true a las funciones ya que esta funcion está hecha para f_list
void aux_stat(char ** command)
{
    struct stat attr;
    int i, args_pos = 0;
    bool in_files = 0;
    //reservo memoria para las listas de archivos y argumentos
    char *file = MALLOC, **args = MALLOC_PTR;
    if (!file || !args) { perror("Error al asignar memoria"); freeAll(2, file, args); return; }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++) args[i] = NULL;

    for (i = 1; command[i]; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long") && strcmp(command[i], "-link") && strcmp(command[i], "-acc")) in_files = 1;

        if (!in_files)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[args_pos] = MALLOC;
            if (!args[args_pos]) { perror("Error al asignar memoria"); freeAllRec(1, args); freeAll(1, file); return; }
            initializeString(args[args_pos]); //para evitar fallos con el strcmp

            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[args_pos++], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[args_pos++], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[args_pos++], "acc");
            //si se repite un parámetro se libera la posicion ya que no se añadirá nada, tampoco incrementa args_pos
            else { freeAll(1, args[args_pos]); args[args_pos] = NULL; }
        }       
        else strcpy(file, command[i]);
    }

    if (includesString("long", args) && !stat(file, &attr)) printStat(file, attr, "long", includesString("link", args), true);
    else if (includesString("acc", args) && !stat(file, &attr)) printStat(file, attr, "acc", includesString("link", args), true);
    else if (includesString("link", args) && !stat(file, &attr)) printStat(file, attr, "link", true, true);
    else if (!stat(file, &attr)) printStat(file, attr, "few", includesString("acc", args), true);

    freeAllRec(1, args); freeAll(1, file);
}

void printAsStat(char * dir, char ** args)
{
    char *command = MALLOC, **full_command = MALLOC_PTR;
    if (!command || !full_command) { perror("Error al asignar memoria"); freeAll(2, command, full_command); return; }

    strcpy(command, "stat ");
    if (includesString("long", args)) strcat(command, "-long ");
    if (includesString("acc", args)) strcat(command, "-acc ");
    if (includesString("link", args)) strcat(command, "-link ");
    strcat(command, dir);

    TrocearCadena(command, full_command);
    aux_stat(full_command);

    freeAll(2, command, full_command);
}

void listDirElements(char * _dir, char ** args, char mode, bool hid, bool deltree /*deltree utiliza esta funcion para eliminar un directorio no vacío*/) 
{
    char *subroute = MALLOC;
    struct dirent *entry;
    //se crea una variable tipo directorio que almacena las caracteristicas del directorio de entrada
    DIR *dir = opendir(_dir);

    if (!dir || !subroute)  { !dir? fprintf(stderr, "Error al acceder a %s: %s\n", _dir, strerror(errno)):perror("Error al asignar memoria");
                              freeAll(1, subroute); return; }

    //se utiliza este modo de recursividad en deltree
    if (mode == 'B')
    {
        //primero crear subroute y luego hacerle isdir
        while ((entry = readdir(dir))) 
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            if (isDir(subroute) && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) listDirElements(subroute, args, mode, hid, deltree);
        }

        rewinddir(dir);
        if (!deltree) printf("************%s\n", _dir);
        //luego listo todos los elementos del directorio
        //sin deltree no puede ser ni . ni .. ni archivo oculto (excepto hid), con deltree los archivos ocultos si se iteran, pero . y .. no
        while ((entry = readdir(dir))) if ((!deltree && (entry->d_name[0]!='.' || hid)) || (deltree && (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")))) 
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            if (!deltree) printAsStat(subroute, args);
            else 
            {
                if (isDir(entry->d_name)) { if (rmdir(subroute)) fprintf(stderr, "Imposible borrar %s: %s\n", entry->d_name, strerror(errno)); }
                else if (remove(subroute)) fprintf(stderr, "Imposible borrar %s: %s\n", entry->d_name, strerror(errno));
            }
        }
    }
    else if (mode == 'A')
    {
        printf("************%s\n", _dir);
        //primero listo todos los elementos del directorio
        while ((entry = readdir(dir))) if (entry->d_name[0]!='.' || hid)
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            printAsStat(subroute, args);
        } 

        //vuelvo a recorrer el directorio y si el elemento es un directorio se imprimen sus elementos de la misma forma
        rewinddir(dir);
        while ((entry = readdir(dir))) 
        {
            snprintf(subroute, MAX_PROMPT, "%s/%s", _dir, entry->d_name);
            if (isDir(subroute) && strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) listDirElements(subroute, args, mode, hid, deltree);
        }
    }
    else
    {
        printf("************%s\n", _dir);
        //va imprimiendo uno por uno los elementos del directorio, entrando a un subdirectorio y recorriendo este si asi se necesitara
        while ((entry = readdir(dir))) if (entry->d_name[0]!='.' || hid) printAsStat(entry->d_name, args);
    }

    freeAll(1, subroute);
    
    closedir(dir);
}

char *getLastNamePath(char *dir)
{
    char *last_slash = strrchr(dir, '/');
    return last_slash? last_slash + 1:dir;
}

void initializeString(char * string) { string[0] = '\0'; }

bool isDir(char * _dir)
{
    struct stat attr;
    if (!stat(_dir, &attr)) return S_ISDIR(attr.st_mode);
    else return false;
}