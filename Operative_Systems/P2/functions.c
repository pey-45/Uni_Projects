#include "functions.h"

#define PERMISSIONS 0644 //para memoria compartida

void f_authors(char ** command)
{
    //si tiene de argumento -l imprime los logins
    if (command[1] && !strcmp(command[1], "-l")) printf("pablo.manzanares.lopez@udc.es\nalejandro.rodriguezf@udc.es\n");
    //si tiene de argumento -n imprime los nombres
    else if (command[1] && !strcmp(command[1], "-n")) printf("Pablo Manzanares Lopez\nAlejandro Rodriguez Franco\n");
    //en cualquier otro caso imprime los nombres y logins
    else printf("Pablo Manzanares Lopez: pablo.manzanares.lopez@udc.es\nAlejandro Rodriguez Franco: alejandro.rodriguezf@udc.es\n");
}

void f_pid(char ** command)
{
    //si hay argumentos y ese argumento es -p imprime el pid del padre del shell, en cualquier otro caso imprime el pid del shell
    bool p = command[1] && !strcmp(command[1], "-p");
    printf("Pid del %sshell: %ld\n", p? "padre del ":"", p? (long)getppid():(long)getpid());
}

void f_chdir(char ** command)
{
    //si no hay argumentos imprime el directorio actual
    if (!command[1]) printCurrentDir();
    //si el argumento no es un directorio valido salta error, en cualquier otro caso cambia al directorio dado
    else if (chdir(command[1])) perror("Imposible cambiar directorio");
}

void f_time(char ** command)
{
    //se guarda en el struct local los datos referentes al horario actual
    time_t t; time(&t); struct tm *local = localtime(&t);

    //se imprime la hora o la fecha dependiendo del comando
    !strcmp(command[0], "time")? printf("%02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec):
                                 printf("%02d/%02d/%d\n", local->tm_mday, local->tm_mon+1, local->tm_year+1900);
}

void f_hist(char ** command, tList * command_history)
{
    //si hay argumento y es -c se borra la lista
    if (command[1] && !strcmp(command[1], "-c")) deleteList(command_history);
    //si hay argumento y no es -c hay que comprobar si lo que hay despues es un numero
    else if (command[1] && command[1][0]=='-' && isDigitString(command[1]+1 /*sin el guion*/)) printHistUntil(atoi(command[1]+1), *command_history);
    //en cualquier otro caso se imprime el historial
    else printHistUntil(listLength(*command_history), *command_history);
}

void f_command(char ** command, tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory) 
{
    char *string = MALLOC, **strings = MALLOC_PTR;
    int current_index = 1;
    tPosL i;

    if (!command[1] || !isDigitString(command[1]) || !string || !strings) 
    {
        //si el error fue ocasionado por un error de memoria no se imprime nada
        if (command[1] && isDigitString(command[1])) perror("Error al asignar memoria");
        //si no hay argumentos o el argumento no es un número se imprime el historial
        else printHistUntil(listLength(*command_history), *command_history);

        freeAll(2, string, strings); return;
    }

    //se recorre la lista de historial hasta el numero dado o nulo si no se encuentra
    for (i = first(*command_history); i && current_index++ < atoi(command[1]); i = next(i));

    //si se encuentra se ejecuta ese comando
    if (i)
    {
        strcpy(string, getItem(i));
        printf("Ejecutando hist (%s): %s\n", command[1], string);
        TrocearCadena(string, strings);
        processCommand(strings, command_history, open_files, memory, shared_memory, mmap_memory);
    } 
    else printf("No hay elemento %s en el histórico\n", command[1]);

    freeAll(2, string, strings);
}

void f_open(char ** command, tList * open_files, bool show_message)
{
    int i, df, mode = 0;
    char *mode_c = MALLOC, *output = MALLOC;

    if (!command[1] || !mode_c || !output)
    {
        //si no hay argumentos se imprime la lista de ficheros abiertos
        !command[1]? printOpenListByDF(*open_files):perror("Error al asignar memoria");
        freeAll(2, mode_c, output); return;
    }

    //para evitar saltos condicionales dependientes de variables sin inicializar
    initializeString(mode_c);

    //se itera desde el segundo argumento asignando los valores correspondientes
    for (i=2; command[i]; ++i)
    {
        if      (!strcmp(command[i],"cr")) { mode|=O_CREAT;  strcpy(mode_c, "O_CREAT"); }
        else if (!strcmp(command[i],"ex")) { mode|=O_CREAT;  strcpy(mode_c, "O_EXCL"); }
        else if (!strcmp(command[i],"ro")) { mode|=O_RDONLY; strcpy(mode_c, "O_RDONLY"); }
        else if (!strcmp(command[i],"wo")) { mode|=O_WRONLY; strcpy(mode_c, "O_WRONLY"); }
        else if (!strcmp(command[i],"rw")) { mode|=O_RDWR;   strcpy(mode_c, "O_RDWR"); }
        else if (!strcmp(command[i],"ap")) { mode|=O_APPEND; strcpy(mode_c, "O_APPEND"); }
        else if (!strcmp(command[i],"tr")) { mode|=O_TRUNC;  strcpy(mode_c, "O_TRUNC"); }
        else break;
    }

    //se intenta abrir y no se puede da error
    if ((df=open(command[1], mode, 0777))==-1) fprintf(stderr, "Imposible abrir %s: %s\n", command[1], strerror(errno));
    //si no simplemente se abrio y se añade a la lista de ficheros abiertos con su descriptor y modo
    else
    {
        sprintf(output, "Descriptor: %d -> %s %s", df, command[1], mode_c);
        insertItem(output, NULL, open_files);
        if (show_message) printf("Anadida entrada %d a la tabla ficheros abiertos\n", df);
    }

    freeAll(2, mode_c, output);
}

void f_close (char ** command, tList * open_files)
{
    int df;

    //si no hay argumentos o este no es un numero se imprime la lista de archivos abiertos
    if (!command[1] || !isDigitString(command[1])) { printOpenListByDF(*open_files); return; }

    //se intenta cerrar y no se puede da error
    if (close(df = atoi(command[1]))==-1) perror("Imposible cerrar descriptor");
    //si no simplemente se cerró y se elimina de la lista de ficheros abiertos
    else deleteAtPosition(getPosByDF(df, *open_files), open_files);
}

void f_dup(char ** command, tList * open_files)
{
    int df;
    tPosL pos;
    char *output = MALLOC, *p = MALLOC, *mode = MALLOC, *aux = MALLOC, **aux_strings = MALLOC_PTR;
    initializeString(p);

    if (!output || !p || !mode || !aux || !aux_strings || !command[1] || !isDigitString(command[1]))
    {
        //si no hay argumentos o no es un numero se imprime la lista de archivos abiertos
        if (!command[1] || !isDigitString(command[1])) printOpenListByDF(*open_files);
        //si no es un error de memoria
        else perror("Error al asignar memoria");

        freeAll(5, output, p, mode, aux, aux_strings); return;
    }
    //el numero se guarda en df
    df = atoi(command[1]);

    /*guardamos la posicion y si existe almacenamos el string en una variable auxiliar 
    para separarla en un array de cadenas y manipularlas mejor*/
    pos = getPosByDF(df, *open_files);
    if (pos) strcpy(aux, getItem(pos));
    else { perror("Imposible duplicar descriptor"); freeAll(5, output, p, mode, aux, aux_strings); return; }

    TrocearCadena(aux, aux_strings);

    //se itera desde el nombre del archivo
    for (int i = 3; aux_strings[i]; ++i)
    {
        //la tercera posicion se tiene que leer como archivo, aunque el string sea uno de los modos
        //si se lee un modo se copia y se deja de buscar
        if (i != 3 &&
           (!strcmp(aux_strings[i], "O_CREAT") ||
            !strcmp(aux_strings[i], "O_EXCL") ||
            !strcmp(aux_strings[i], "O_RDONLY") ||
            !strcmp(aux_strings[i], "O_WRONLY") ||
            !strcmp(aux_strings[i], "O_RDWR") ||
            !strcmp(aux_strings[i], "O_APPEND") ||
            !strcmp(aux_strings[i], "O_TRUNC")))   
        { strcpy(mode, aux_strings[i]); break; }


        strcat(p, aux_strings[i]); strcat(p, " ");
    }
    //se quita el espacio sobrante
    p[strlen(p)-1] = '\0';

    //se coloca todo en su lugar y se inserta en la lista de archivos abiertos
    sprintf(output, "Descriptor: %d -> dup %d (%s) %s", dup(df), df, p, mode);
    insertItem(output, NULL, open_files);

    freeAll(5, output, p, mode, aux, aux_strings);
}

void f_listopen(char ** command, tList open_files)
{
    //si tiene argumento y es un numero se imprime hasta ahi
    if (command[1] && isDigitString(command[1])) printOpenListByDFUntil(atoi(command[1]), open_files);
    //en cualquier otro caso se imprime todo
    else printOpenListByDF(open_files);
}

void f_infosys()
{
    //se guardan los datos del dispositivo en un struct y se imprimen
    struct utsname unameData; uname(&unameData);
    printf("%s (%s), OS: %s-%s-%s\n", unameData.nodename, unameData.machine, unameData.sysname, unameData.release, unameData.version);
}

void f_help(char ** command)
{
    //se muestra la ayuda correspondiente a cada comando
    if (!command[1]) printf("help [cmd|-lt|-T topic]: ayuda sobre comandos\n\tComandos disponibles:\nauthors pid chdir " 
    "date time hist command open close dup listopen infosys help quit exit bye create stat list delete deltree\n");
    else if (!strcmp(command[1], "authors")) printf("authors [-n|-l]: Muestra los nombres y/o logins de los autores\n");
    else if (!strcmp(command[1], "pid")) printf("pid [-p]: Muestra el pid del shell o de su proceso padre\n");
    else if (!strcmp(command[1], "chdir")) printf("chdir [dir]: Cambia (o muestra) el directorio actual del shell\n");
    else if (!strcmp(command[1], "date")) printf("date: Muestra la fecha actual\n");
    else if (!strcmp(command[1], "time")) printf("time: Muestra la hora actual\n");
    else if (!strcmp(command[1], "hist")) printf("hist [-c|-N]: Muestra (o borra) el historico de comandos\n\t-N: muestra los "
    "N primeros\n\t-c: borra el historico\n");
    else if (!strcmp(command[1], "command")) printf("command [-N]: Repite el comando N (del historico)\n");
    else if (!strcmp(command[1], "open")) printf("open fich m1 m2...:	Abre el fichero fich. y lo anade a la lista de ficheros "
    "abiertos del shell\nm1, m2..es el modo de apertura (or bit a bit de los siguientes)\n\tcr: O_CREAT\tap: O_APPEND\n\tex: O_EXCL"
    "\tro: O_RDONLY\n\trw: O_RDWR\two: O_WRONLY\n\ttr: O_TRUNC\n");
    else if (!strcmp(command[1], "close")) printf("close df: Cierra el descriptor df y elimina el correspondiente fichero de la "
    "lista de ficheros abiertos\n");
    else if (!strcmp(command[1], "dup")) printf("dup df: Duplica el descriptor de fichero df y anade una nueva entrada a la lista "
    "ficheros abiertos\n");
    else if (!strcmp(command[1], "listopen")) printf("listopen [n]: Lista los ficheros abiertos (al menos n) del shell\n");
    else if (!strcmp(command[1], "infosys")) printf("infosys: Muestra informacion de la maquina donde corre el shell\n");
    else if (!strcmp(command[1], "help")) printf("help [cmd|-lt|-T]: Muestra ayuda sobre los comandos\n\t-lt: lista topics de ayuda"
    "\n\t-T topic: lista comandos sobre ese topic\n\tcmd: info sobre el comando cmd\n");
    else if (!strcmp(command[1], "quit") || !strcmp(command[1], "exit") || !strcmp(command[1], "bye")) printf("%s: Termina la "
    "ejecucion del shell\n", command[1]);
    else if (!strcmp(command[1], "create")) printf("create [-f] [name]: Crea un directorio o un fichero (-f)\n");
    else if (!strcmp(command[1], "stat")) printf("stat [-long][-link][-acc] [name1] [name2] [...]:	lista ficheros\n\t-long: listado largo"
	"\n\t-acc: acesstime\n\t-link: si es enlace simbolico, el path contenido\n");
    else if (!strcmp(command[1], "list")) printf("list [-reca] [-recb] [-hid][-long][-link][-acc] [n1] [n2] [...]: lista contenidos de directorios"
	"\n\t-hid: incluye los ficheros ocultos\n\t-recb: recursivo (antes)\n\t-reca: recursivo (despues)\n\tresto parametros como stat\n");
    else if (!strcmp(command[1], "delete")) printf("delete [name1] [name2] [...]: Borra ficheros o directorios vacios\n");
    else if (!strcmp(command[1], "deltree")) printf("deltree [name1] [name2] [...]: Borra ficheros o directorios no vacios recursivamente\n");
    else if (!strcmp(command[1], "malloc")) printf("malloc [-free] [tam]: asigna un bloque memoria de tamano tam con malloc \n\t-free:"
    "desasigna un bloque de memoria de tamano tam asignado con malloc\n");
    else if (!strcmp(command[1], "shared")) printf("shared [-free|-create|-delkey] cl [tam]: asigna memoria compartida con clave cl en el programa"
	"\n\t-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam"
	"\n\t-free cl: desmapea el bloque de memoria compartida de clave cl"
	"\n\t-delkey clelimina del sistema (sin desmapear) la clave de memoria cl\n");
    else if (!strcmp(command[1], "mmap")) printf("mmap [-free] fich prm: mapea el fichero fich con permisos prm \n\t-free fich: desmapea el ficherofich\n");
    else if (!strcmp(command[1], "read")) printf("read fiche addr cont: lee cont bytes desde fich a la direccion addr\n");
    else if (!strcmp(command[1], "write")) printf("write [-o] fiche addr cont: escribe cont bytes desde la direccion addr a fich (-o sobreescribe)\n");
    else if (!strcmp(command[1], "memdump")) printf ("memdump addr cont: vuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr\n");
    else if (!strcmp(command[1], "memfill")) printf("memfill addr cont byte: llena la memoria a partir de addr con byte\n");
    else if (!strcmp(command[1], "mem")) printf("mem [-blocks|-funcs|-vars|-all|-pmap] ..: muestra muestra detalles de la memoria del proceso"
	"\n\t-blocks: los bloques de memoria asignados"
	"\n\t-funcs: las direcciones de las funciones"
	"\n\t-vars: las direcciones de las variables"
	"\n\t:-all: todo"
	"\n\t-pmap: muestra la salida del comando pmap(o similar)\n");
    else if (!strcmp(command[1], "recurse")) printf("recurse [n]: invoca a la funcion recursiva n veces\n");
    else printf("%s no encontrado\n", command[1]);
}

void f_quit(tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory) 
{ 
    deleteList(command_history); 
    deleteList(open_files); 
    deleteList(memory); 
    deleteList(shared_memory); 
    deleteList(mmap_memory); 
    exit(0); 
}

void f_invalid() { perror("No ejecutado"); }

void f_create(char ** command, tList * open_files)
{
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (!command[1] || (!strcmp(command[1], "-f") && !command[2]) || !string || !strings)
    {
        if (!command[1] || (!strcmp(command[1], "-f") && !command[2])) printCurrentDir();
        else perror("Error al asignar memoria");
        freeAll(2, string, strings); return;
    }

    if (!strcmp(command[1], "-f"))
    {
        sprintf(string, "open %s cr", command[2]);
        TrocearCadena(string, strings);
        f_open(strings, open_files, false);
    }
    else if (mkdir(command[1], 0777)) perror("Imposible crear directorio");
    
    freeAll(2, string, strings);
}

void f_stat(char ** command)
{
    struct stat attr;
    //reservo memoria para las listas de archivos y argumentos
    char **files = MALLOC_PTR, **args = MALLOC_PTR;
    bool in_files = 0;
    int i, args_pos = 0, files_pos = 0;

    if (!files || !args) { perror("Error al asignar memoria"); freeAll(2, files, args); return; }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++) { files[i] = NULL; args[i] = NULL; }

    for (i = 1; command[i]; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long") && strcmp(command[i], "-link") && strcmp(command[i], "-acc")) in_files = 1;

        if (!in_files)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[args_pos] = MALLOC;
            if (!args[args_pos]) { perror("Error al asignar memoria"); freeAllRec(2, files, args); return; }
            initializeString(args[args_pos]); //para evitar fallos con el strcmp

            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[args_pos++], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[args_pos++], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[args_pos++], "acc");
            //si se repite un parámetro se libera la posicion ya que no se añadirá nada, tampoco incrementa args_pos
            else freeAll(1, args[args_pos]);
        }       
        else
        {
            files[files_pos] = MALLOC;
            if (!files[files_pos]) { perror("Error al asignar memoria"); freeAllRec(2, files, args); return; }
            strcpy(files[files_pos++], command[i]);
        }
    }

    if (!files[0]) { printCurrentDir(); freeAllRec(2, files, args); return; }

    for (i = 0; i < files_pos; i++)
    {
        if (!stat(files[i], &attr))
        {
            if (includesString("long", args)) printStat(files[i], attr, "long", includesString("link", args), false);
            else if (includesString("acc", args)) printStat(files[i], attr, "acc", includesString("link", args), false);
            else if (includesString("link", args)) printStat(files[i], attr, "link", includesString("link", args), false);
            else printStat(files[i], attr, "few", includesString("link", args), false);
        }
        else fprintf(stderr, "Error al acceder a %s: %s\n", files[i], strerror(errno));
    }

    freeAllRec(2, files, args);
}

void f_list(char ** command)
{
    if (!command[1]) { printCurrentDir(); return; } 

    int i, args_pos = 0, dirs_pos = 0;
    char **dirs = MALLOC_PTR, **args = MALLOC_PTR, recAorB = '\0';
    bool hid = false, in_dirs = false;

    if (!dirs || !args) { perror("Error al asignar memoria"); freeAll(2, dirs, args); return; }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++) { dirs[i] = NULL; args[i] = NULL; }

    for (i = 1; command[i]; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long") && strcmp(command[i], "-link") && strcmp(command[i], "-acc") && strcmp(command[i], "-reca") 
            && strcmp(command[i], "-recb") && strcmp(command[i], "-hid")) in_dirs = 1;

        if (!in_dirs)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[args_pos] = MALLOC;
            initializeString(args[args_pos]);
            if (!args[args_pos]){ perror("Error al asignar memoria"); freeAllRec(2, dirs, args); return; }
            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[args_pos++], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[args_pos++], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[args_pos++], "acc");
            else if (!strcmp(command[i], "-reca")) { recAorB = 'A'; args_pos++; }
            else if (!strcmp(command[i], "-recb")) { recAorB = 'B'; args_pos++; }
            else if (!strcmp(command[i], "-hid")) { hid = true; args_pos++; }
            //si se repite un parámetro entre long, acc y link se libera la posicion ya que no se añadirá nada, tampoco incrementa args_pos
            else freeAll(1, args[args_pos]);
        }       
        else
        {
            dirs[dirs_pos] = MALLOC;
            if (!dirs[dirs_pos]) { perror("Error al asignar memoria"); freeAllRec(2, dirs, args); return; }
            strcpy(dirs[dirs_pos++], command[i]);
        }
    }

    if (!dirs[0]) { printCurrentDir(); freeAllRec(2, dirs, args); return; }

    for (i = 0; i < dirs_pos; i++) listDirElements(dirs[i], args, recAorB, hid, false);
    
    freeAllRec(2, dirs, args);
}

void f_delete(char ** command)
{
    struct stat attr;
    char sure, aux;
    int i;

    if (!command[1]) { printCurrentDir(); return; }

    if (!strcmp(command[0], "deltree"))
    {
        printf("Esta es una acción destructiva, deseas continuar [y/n]? ");
        scanf("%c", &sure); scanf("%c", &aux); //aux para evitar desbordamiento de buffer
        if (sure!='y' && sure !='Y') return;
    }
    
    for (i = 1; command[i]; i++)
    {
        if (lstat(command[i], &attr)) fprintf(stderr, "Imposible borrar %s: %s\n", command[i], strerror(errno));
        else if (S_ISDIR(attr.st_mode)) 
        {
            if (!strcmp(command[0], "deltree")) listDirElements(command[i], NULL, 'B', false, true);
            if (rmdir(command[i])) fprintf(stderr, "Imposible borrar %s: %s\n", command[i], strerror(errno));
        }
        else if (remove(command[i])) fprintf(stderr, "Imposible borrar %s: %s\n", command[i], strerror(errno));
    }
}

void f_malloc(char ** command, tList * memory)
{
    void *ptr;
    char *aux = MALLOC, *item = MALLOC, **full_item = MALLOC_PTR, *ptr_string = MALLOC;
    tPosL pos;
    time_t t; 
    time(&t); 
    struct tm *local = localtime(&t);

    if (!command[1] || 
        (!isDigitString(command[1]) && strcmp(command[1], "-free")) || 
        (!strcmp(command[1], "-free") && (!command[2] || !isDigitString(command[2])))) 
        { 
            printf("******Lista de bloques asignados malloc para el proceso %ld\n", (long)getpid()); 
            printList(memory); 
            return;
        }

    if (strcmp(command[1], "-free")) 
    {
        ptr = malloc(atoi(command[1]));
        sprintf(ptr_string, "%p", (void *)ptr);
        strftime(aux, MAX_PROMPT, "%b %d %H:%M", local);
        snprintf(item, MAX_PROMPT, "%20s%17s%14s%7s", ptr_string, command[1], aux, "malloc");
        insertItem(item, NULL, memory);
        printf("Asignados %s bytes en %s\n", command[1], ptr_string);
    }
    else
    {
        for (pos = first(*memory); pos != NULL; pos = next(pos))
        {
            strcpy(aux, getItem(pos));
            TrocearCadena(aux, full_item);
            if (!strcmp(full_item[1], command[2]))
            {
                unsigned long long int memdir = strtoull(full_item[0], NULL, 16);
                ptr = (void*)memdir;
                free(ptr);
                deleteAtPosition(pos, memory);
                return;
            }
        }
    }
}

void f_shared(char ** command, tList * shared_memory)
{
    int i, size, ptr_free;
    char *item = MALLOC, *aux = MALLOC, *ptr_string = MALLOC, **full_item = MALLOC_PTR;
    key_t key;
    void *ptr;
    time_t t; time(&t); struct tm *local = localtime(&t);
    tPosL pos;


    if (!command[1]) 
    { 
        printf("******Lista de bloques asignados shared para el proceso %ld\n", (long)getpid());
        printList(shared_memory); return;
    }
    for (i = 1; command[i]!=NULL; i++)
    {
        if (!strcmp("-create", command[i]) || !strcmp("-free", command[i]) || !strcmp("-delkey", command[i])) continue;
        else if ((!strcmp("-create", command[1]) && command[i] && command[i+1] && isDigitString(command[i]) && isDigitString(command[i+1])) ||
                 ((!strcmp("-free", command[1]) || !strcmp("-delkey", command[1])) && command[i] && isDigitString(command[i])))
        {
            key = atoi(command[i]);
            if (!strcmp("-create", command[1])) size = atoi(command[i+1]);
            break;
        }
        else 
        { 
            printf("******Lista de bloques asignados shared para el proceso %ld\n", (long)getpid());
            printList(shared_memory); 
            return; 
        }
    }

    if (!strcmp(command[1], "-create")) 
    {
        key=(key_t) strtoul(command[2],NULL,10);
        size=(size_t) strtoul(command[3],NULL,10);
        if (!size) { printf ("No se asignan bloques de 0 bytes\n"); return; }
        if ((ptr=getShm(key, size)))
        {		    
            sprintf(ptr_string, "%p", (void *)ptr);
            printf ("Asignados %lu bytes en %p\n", (unsigned long) size, ptr);  
            strftime(aux, MAX_PROMPT, "%b %d %H:%M", local);
            snprintf(item, MAX_PROMPT, "%20s%17s%14s%13s%d%c", ptr_string, command[3], aux, "shared (key ", key, ')');
            insertItem(item, NULL, shared_memory);
        }
        else printf ("Imposible asignar memoria compartida clave %lu: %s\n", (unsigned long)key, strerror(errno));
    }
    else if (!strcmp(command[1], "-free")) 
    {
        for (pos = first(*shared_memory); pos != NULL; pos = next(pos))
        {
            strcpy(aux, getItem(pos));
            TrocearCadena(aux, full_item);
            full_item[7][strlen(full_item[7])-1] = '\0';

            if (!strcmp(full_item[7], command[2]))
            {
                if ((ptr_free=shmget(key, (size_t)atoi(full_item[1]), PERMISSIONS)) && shmctl(ptr_free, IPC_RMID, NULL) == -1)
                { perror("Error al liberar la memoria compartida"); return; }
            }
            else continue;

            printf("Memoria compartida liberada correctamente.\n");
            deleteAtPosition(pos, shared_memory);
            return;
        }
    }
    else if (!strcmp(command[1], "-delkey")) 
    {
        if ((key= (key_t)strtoul(command[2],NULL,10)) == IPC_PRIVATE)
        { printf ("delkey necesita clave_valida\n"); return; }
        if ((ptr_free=shmget(key,0,0666))==-1)
        { perror ("shmget: imposible obtener memoria compartida"); return; }
        if (shmctl(ptr_free,IPC_RMID,NULL)==-1)
            perror ("shmctl: imposible eliminar id de memoria compartida\n");
    }
    else fprintf(stderr, "Imposible asignar memoria compartida clave %s: %s\n", command[1], strerror(errno));
}

void f_mmap(char ** command, tList * mmap_memory)
{
    int i, protection = 0, file_size;
    void *ptr;
    tPosL pos;
    char *string = MALLOC, **strings = MALLOC_PTR;

    if (!command[1]) 
    { 
        printf("******Lista de bloques asignados mmap para el proceso %ld\n", (long)getpid());
        printList(mmap_memory); 
        return;
    }

    if (strcmp(command[1], "-free"))
    {
        for (i = 0; i < 3 && command[2] && command[2][i]; i++)
        {
            if (command[2][i]=='r') protection|=PROT_READ;
            if (command[2][i]=='w') protection|=PROT_WRITE;
            if (command[2][i]=='x') protection|=PROT_EXEC;
        }

        if ((ptr = mmap_file(command[1], protection, mmap_memory)))
            printf("Fichero %s mapeado en %p\n", command[1], ptr);
        else
            perror("Imposible mapear fichero");
    }
    else
    {
        for (pos = first(*mmap_memory); pos != NULL; pos = next(pos))
        {
            strcpy(string, getItem(pos));
            TrocearCadena(string, strings);
            if (!strcmp(strings[5], command[2]))
            {
                file_size = atoi(strings[1]);
                break;
            }
        }

        if (!pos) 
        {
            printf("Fichero %s no mapeado\n", command[2]);
            return;
        }

        unsigned long long int memdir = strtoull(strings[0], NULL, 16);
        ptr = (void*)memdir;
        munmap(ptr, file_size);
        
        deleteAtPosition(pos, mmap_memory);
    }
}

void f_read(char ** command)
{
    int cnt = 0;
    void *ptr;
    struct stat attr;
    unsigned long long memdir;

    if (!command[2]) { printf("Faltan parámetros\n"); return; }

    if (stat(command[1], &attr)) { printf("No se ha podido hacer stat\n"); return; }            

    if (!command[3]) cnt = attr.st_size;
    else if (isDigitString(command[3])) cnt = atoi(command[3]);

    memdir = strtoull(command[2], NULL, 16);
    ptr = (void*)memdir;

    if (readFile(command[1], ptr, cnt) == -1)
        perror("Imposible leer fichero");
    else
        printf("Leídos %d bytes de %s en %s\n", cnt, command[1], command[2]);
}

void f_write(char ** command)
{    
    int cnt = 0;
    void *ptr;
    bool o;
    unsigned long long memdir;

    if ((!command[1] ||
         (command[1] && !strcmp(command[1], "-o") && !command[4])) ||
         (command[1] && strcmp(command[1], "-o") && !command[3])) 
         { printf("Faltan parámtros\n"); return; }

    o = !strcmp(command[1], "-o");

    cnt = atoi(command[o? 4:3]);

    memdir = strtoull(command[o? 3:2], NULL, 16);
    ptr = (void*)memdir;

    if (writeFile(command[o? 2:1], ptr, cnt, o)==-1)
        perror("Imposible escribir fichero");
    else 
        printf("Escritos %s bytes en %s desde %s\n", command[o? 4:3], command[o? 2:1], command[o? 3:2]);
}

void f_memdump(char ** command)
{
    int cnt = 25, i, j;
    char *buffer;
    unsigned long long memdir;

    if (!command[1]) return;

    if (command[2] && isDigitString(command[2])) 
        cnt = atoi(command[2]);
    
    memdir = strtoull(command[1], NULL, 16);

    buffer = (char*)malloc(cnt);
    if (!buffer) { perror("Error al asignar memoria"); return; }

    for (i = 0; i < cnt; i++) 
    {
        if ((memdir + i) < memdir) 
        {
            fprintf(stderr, "Dirección de memoria fuera de límites.\n");
            free(buffer);
            return;
        }
        buffer[i] = *((char*)(memdir + i));
    }

    for (i = 0; i*25 < cnt; i++) 
    {
        for (j = 0; j < 25 && i*25+j < cnt; j++) 
        {
            if (!buffer[i*25+j]) printf("   ");
            else printf("%3c", buffer[i*25+j]);
        }
        printf("\n");  

        for (j = 0; j < 25 && i*25+j < cnt; j++) 
        {
            if (!buffer[i*25+j]) printf(" 00");
            else printf("%3X", buffer[i*25+j]);
        }
        printf("\n");
    }
    printf("\n");
}

void f_memfill(char ** command)
{
    int cnt = 128, i, byte = 41;
    char *buffer;
    unsigned long long memdir;

    if (!command[1]) return;

    if (command[2] && isDigitString(command[2])) 
        cnt = atoi(command[2]);

    if (command[3] && isDigitString(command[3]))
        byte = atoi(command[3]);

    memdir = strtoull(command[1], NULL, 16);
    buffer = memdir;

    for (i = 0; i < cnt; i++) 
        buffer[i] = byte;

    printf("Llenando %d bytes de memoria con el byte %c(%d) a partir de la dirección %s", cnt, byte, byte, command[1]);
}




void processCommand(char ** command, tList * command_history, tList * open_files, tList * memory, tList * shared_memory, tList * mmap_memory)
{
    //se ejecuta el comando correspondiente a command
    if (!command[0]) return;
    if (!strcmp(command[0], "authors")) f_authors(command);
    else if (!strcmp(command[0], "pid")) f_pid(command);
    else if (!strcmp(command[0], "chdir")) f_chdir(command);
    else if (!strcmp(command[0], "date") || !strcmp(command[0], "time")) f_time(command);
    else if (!strcmp(command[0], "hist")) f_hist(command, command_history);
    else if (!strcmp(command[0], "command")) f_command(command, command_history, open_files, memory, shared_memory, mmap_memory);
    else if (!strcmp(command[0], "open")) f_open(command, open_files, true);
    else if (!strcmp(command[0], "close")) f_close(command, open_files);
    else if (!strcmp(command[0], "dup")) f_dup(command, open_files);
    else if (!strcmp(command[0], "listopen")) f_listopen(command, *open_files);
    else if (!strcmp(command[0], "infosys")) f_infosys();
    else if (!strcmp(command[0], "help")) f_help(command);
    else if (!strcmp(command[0], "quit") || !strcmp(command[0], "exit") || !strcmp(command[0], "bye")) f_quit(command_history, open_files, memory, shared_memory, mmap_memory);
    else if (!strcmp(command[0], "clear")) system("clear");
    else if (!strcmp(command[0], "create")) f_create(command, open_files);
    else if (!strcmp(command[0], "stat")) f_stat(command);
    else if (!strcmp(command[0], "list")) f_list(command);
    else if (!strcmp(command[0], "delete") || !strcmp(command[0], "deltree")) f_delete(command);
    else if (!strcmp(command[0], "malloc")) f_malloc(command, memory);
    else if (!strcmp(command[0], "shared")) f_shared(command, shared_memory);
    else if (!strcmp(command[0], "mmap")) f_mmap(command, mmap_memory);
    else if (!strcmp(command[0], "read")) f_read(command);
    else if (!strcmp(command[0], "write")) f_write(command);
    else if (!strcmp(command[0], "memdump")) f_memdump(command);
    else if (!strcmp(command[0], "memfill")) f_memfill(command);
    else if (!strcmp(command[0], "mem")) return; 
    else if (!strcmp(command[0], "recurse")) return; 
    else f_invalid();
}
