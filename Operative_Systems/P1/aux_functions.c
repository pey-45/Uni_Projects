#include "aux_functions.h"


int TrocearCadena(char * cadena, char ** trozos)
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
    for (int i = 0; i < (int)strlen(string); ++i)
    {
        if (!isdigit(string[i]))
            return false;
    }
    return true;
}

void printCurrentDir()
{
    char *dir = malloc(MAX_PROMPT*sizeof(char*));

    getcwd(dir, MAX_PROMPT);
    printf("%s\n", dir);

    free(dir);
}

char LetraTF (mode_t m)
{
     switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */ 
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
     }
}

void printLong(char * file, struct stat attr)
{
    int year, month, day, hour, min;
    nlink_t n_link;
    ino_t n_ino;
    mode_t mode;
    char *permissions = malloc(12*sizeof(char*));
    off_t size;
    struct passwd *prop;
    struct group *group;
    
    if (!lstat(file, &attr))
    {
        time_t timestamp = attr.st_mtime;
        struct tm *timeinfo = localtime(&timestamp);
        n_link = attr.st_nlink;
        n_ino = attr.st_ino;
        year = timeinfo->tm_year;
        month = timeinfo->tm_mon;
        day = timeinfo->tm_mday;
        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
        mode = attr.st_mode;
        size = attr.st_size;
        prop = getpwuid(attr.st_uid);
        group = getgrgid(attr.st_gid);
    }
    else
    {
        perror("No se ha podido hacer lstat");
        free(permissions);
        return;
    }
    ConvierteModo(mode, permissions);


    printf("%d/%02d/%02d-%02d:%02d  %lu (%lu) %s %s %s %d %s", 1900+year, month+1, day, hour, min, n_link, n_ino, prop->pw_name, group->gr_name, permissions, (int)size, file);
    //se añade un espacio para añadir el link en caso de necesitarlo

    free(permissions);
}

void printLink(const char *file) 
{
    char *link_path = malloc(MAX_PROMPT*sizeof(char*));
    ssize_t length = readlink(file, link_path, sizeof(link_path) - 1);

    if (length != -1) 
    {
        link_path[length] = '\0'; 
        printf("%s", link_path);
    }
    //si no es link no imprime nada

    free(link_path);
}

void printAcc(const char *file, struct stat attr)
{
    int year, month, day, hour, min;
    if (!lstat(file, &attr))
    {
        time_t timestamp = attr.st_mtime;
        struct tm *timeinfo = localtime(&timestamp);
        year = timeinfo->tm_year;
        month = timeinfo->tm_mon;
        day = timeinfo->tm_mday;
        hour = timeinfo->tm_hour;
        min = timeinfo->tm_min;
    }
    else
    {
        perror("No se ha podido hacer lstat");
        return;
    }

    printf("%d/%02d/%02d-%02d:%02d", 1900+year, month+1, day, hour, min);
    //se añade un espacio para añadir el link en caso de necesitarlo
}

void printFew(const char * file, struct stat attr)
{
    off_t size;
    if (!lstat(file, &attr))
        size = attr.st_size;
    else
    {
        perror("No se ha podido hacer lstat");
        return;
    }

    printf("%d %s", (int)size, file);
}

char * ConvierteModo (mode_t m, char *permisos)
{
    strcpy (permisos,"---------- ");
    
    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';
    
    return permisos;
}

bool includesString(char * string, char ** strings)
{
    if (strings == NULL) return false;
    int i;
    for (i = 0; strings[i]!=NULL; i++) if (!strcmp(string, strings[i])) return true;
    return false;
}