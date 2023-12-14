#include "aux_functions.h"

//libera strings y cadenas de strings con solo elementos nulos
//sirve para trimString
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

int trimString(char * string, char ** strings)
{
    int i;

    if (!(strings[0]=strtok(string," \n\t"))) return 0;
    for (i = 1; (strings[i]=strtok(NULL," \n\t")); ++i);
    return i;
}

void untrimString(char ** strings, char * string)
{
    int i;

    if (!strings[0]) return;

    strcpy(string, strings[0]);
    for (i = 1; strings[i]; i++)
    { 
        strcat(string, " ");
        strcat(string, strings[i]);
    }
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
        trimString(string, strings);
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
        trimString(string, strings);
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
    trimString(string, strings);
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
    for (i = 0; i < limit && getPosByDF(i, L)!=next(getLastPosByDF(L)); i++)
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
        if (!prop || !group) { perror("Error al obtener datos"); freeAll(2, permissions, link_path); return; }
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

void printAsStat(char * dir, char ** args)
{
    char *command = MALLOC, **full_command = MALLOC_PTR;
    if (!command || !full_command) { perror("Error al asignar memoria"); freeAll(2, command, full_command); return; }

    strcpy(command, "stat ");
    if (includesString("long", args)) strcat(command, "-long ");
    if (includesString("acc", args)) strcat(command, "-acc ");
    if (includesString("link", args)) strcat(command, "-link ");
    strcat(command, dir);

    trimString(command, full_command);
    f_stat(full_command);

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
                if (isDir(subroute)) { if (rmdir(subroute)) fprintf(stderr, "Imposible borrar %s: %s\n", entry->d_name, strerror(errno)); }
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

void printList(tList * L)
{
    tPosL pos;
    for (pos = first(*L); pos != NULL; pos = next(pos)) printf("%s\n", getItem(pos));
}

void *getShm (key_t clave, size_t tam)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
             shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    return (p);
}

void *mmap_file (char * fichero, int protection, tList * mmap_memory)
{
    time_t t; time(&t); struct tm *local = localtime(&t);

    char *string = MALLOC, *ptr_string = MALLOC, *aux = MALLOC;
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *ptr;

    if (protection&PROT_WRITE)
          modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
    {
        freeAll(3, string, ptr_string, aux);
        return NULL;
    }
    if ((ptr=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
    {
        freeAll(3, string, ptr_string, aux);
        return NULL;  
    }

    strftime(aux, MAX_PROMPT, "%b %d %H:%M", local);
    sprintf(ptr_string, "%p", (void *)ptr);
    snprintf(string, MAX_PROMPT, "%20s%17ld%14s %s  (descriptor %d)", ptr_string, (long)s.st_size, aux, fichero, df); 
    insertItem(string, NULL, mmap_memory);

    freeAll(3, string, ptr_string, aux);
    return ptr;
}

ssize_t writeFile(char *f, void *p, size_t cont,int overwrite)
{
    ssize_t  n;
    int df,aux, flags= O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
	    return -1;

    if ((n=write(df,p,cont))==-1)
    {
	    aux=errno;
	    close(df);
	    errno=aux;
	    return -1;
    }
    close (df);
    return n;
}

ssize_t readFile(char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t  n;  
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
	    return -1;     
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
	    cont=s.st_size;
    if ((n=read(df,p,cont))==-1)
    {
	    aux=errno;
	    close(df);
	    errno=aux;
	    return -1;
    }
    close (df);
    return n;
}

void doMemPmap() /*sin argumentos*/
 { 
    pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};
   
    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1)
    {
        perror ("Imposible crear proceso");
        return;
    }

    if (pid==0)
    { /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");
      
        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
            if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
                perror("cannot execute vmmap (Mac-OS)");          
      
        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;   
            if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
                perror("cannot execute procstat (FreeBSD)");
         
        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;    
            if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
                perror("cannot execute procmap (OpenBSD)");
         
        exit(1);
    }

    waitpid (pid,NULL,0);
}

void recurse(int n)
{
    char automatico[MAX_PROMPT];
    static char estatico[MAX_PROMPT];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0) recurse(n-1);
}

void show_credentials()
{
    struct passwd *pw;

    pw = getpwuid(getuid());
    printf("Credencial real: %d, ", getuid());
    if (pw) printf("(%s)\n", pw->pw_name);
    else printf("(Desconocido)\n");

    pw = getpwuid(geteuid());
    printf("Credencial efectiva: %d, ", geteuid());
    if (pw) printf("(%s)\n", pw->pw_name);
    else printf("(Desconocido)\n");
}

int findVariable(char * var, char ** e)  /*busca una variable en el entorno que se le pasa como parÃ¡metro*/
{
  	int i = 0;
  	char aux[MAX_PROMPT];
  
    sprintf(aux, "%s=", var);
  
  	while (e[i])
    	if (!strncmp(e[i], aux, strlen(aux)-1)){
            return i;
        }
    	else i++;
  	errno=ENOENT;
  	return -1;
}

int changeVar(char * var, char * valor, char ** e)
{                                                       
  	int pos;
  	char aux[MAX_PROMPT];
   
  	if ((pos=findVariable(var,e))==-1) return -1;

    sprintf(aux, "%s=%s", var, valor);
  	e[pos]=aux;
  	return pos;
}

int stringsSize(char ** strings)
{
    int i;

    for (i = 0; strings[i]; i++);

    return i;
}

char *processStatus(char c)
{
    switch(c)
    {
        case 'R': return "ACTIVO";
        case 'S': return "EN ESPERA";
        case 'D': return "EN ESPERA ININTERRUMPIBLE";
        case 'Z': return "ZOMBI";
        case 'T': return "TERMINADO";
        default: return NULL;
    }
}

struct SEN {
    const char *name;
    int signal_number;
};

static struct SEN sigstrnum[]={   
	{"HUP", SIGHUP},
	{"INT", SIGINT},
	{"QUIT", SIGQUIT},
	{"ILL", SIGILL}, 
	{"TRAP", SIGTRAP},
	{"ABRT", SIGABRT},
	{"IOT", SIGIOT},
	{"BUS", SIGBUS},
	{"FPE", SIGFPE},
	{"KILL", SIGKILL},
	{"USR1", SIGUSR1},
	{"SEGV", SIGSEGV},
	{"USR2", SIGUSR2}, 
	{"PIPE", SIGPIPE},
	{"ALRM", SIGALRM},
	{"TERM", SIGTERM},
	{"CHLD", SIGCHLD},
	{"CONT", SIGCONT},
	{"STOP", SIGSTOP},
	{"TSTP", SIGTSTP}, 
	{"TTIN", SIGTTIN},
	{"TTOU", SIGTTOU},
	{"URG", SIGURG},
	{"XCPU", SIGXCPU},
	{"XFSZ", SIGXFSZ},
	{"VTALRM", SIGVTALRM},
	{"PROF", SIGPROF},
	{"WINCH", SIGWINCH}, 
	{"IO", SIGIO},
	{"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
	{"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
	{"PWR", SIGPWR},
#endif
#ifdef SIGEMT
	{"EMT", SIGEMT},
#endif
#ifdef SIGINFO
	{"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
	{"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
	{"CLD", SIGCLD},
#endif
#ifdef SIGLOST
	{"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
	{"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
	{"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
	{"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
	{"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
	{"WAITING", SIGWAITING},
#endif
 	{NULL,-1},
	};    /*fin array sigstrnum */


int signalValue(char * signal) 
{ 
    int i;

    for (i = 0; sigstrnum[i].name; i++)
  	    if (!strcmp(signal, sigstrnum[i].name))
		    return sigstrnum[i].signal_number;

    return -1;
}


const char *signalName(int signal)
{			
    int i;

    for (i=0; sigstrnum[i].name; i++)
  	    if (signal==sigstrnum[i].signal_number)
		    return sigstrnum[i].name;

    return ("SIGUNKNOWN");
}

//solo funciona con bg_proc
int getPidFromPos(tPosL pos)
{
    char string[MAX_PROMPT];

    strcpy(string, getItem(pos));

    return atoi(strtok(string, " "));
}