#include "functions.h"

void f_authors(char ** command)
{
    //si no tiene argumentos imprime los nombres y logins
    if (command[1]==NULL)
    {
        printf("Pablo Manzanares Lopez: pablo.manzanares.lopez@udc.es\n");
        printf("Alejandro Rodriguez Franco: alejandro.rodriguezf@udc.es\n");
    }
    //si tiene de argumento -l imprime los logins
    else if (!strcmp(command[1], "-l"))
    {
        printf("pablo.manzanares.lopez@udc.es\n");
        printf("alejandro.rodriguezf@udc.es\n");
    }
    //si tiene de argumento -n imprime los nombres
    else if (!strcmp(command[1], "-n"))
    {
        printf("Pablo Manzanares Lopez\n");
        printf("Alejandro Rodriguez Franco\n");
    }
    //si no no imprime nada
}

void f_pid(char ** command)
{
    //si hay argumentos y ese argumento es -p imprime el pid del padre del shell
    if (command[1]!=NULL && strcmp(command[1], "-p")==0)
        printf("Pid del padre del shell: %ld\n", (long)getppid());
    //en cualquier otro caso imprime el pid del shell
    else
        printf("Pid de shell: %ld\n", (long)getpid());
}

void f_chdir(char ** command)
{
    //si no hay argumentos imprime el directorio actual
    if (command[1]==NULL)
        printCurrentDir();
    /*si el argumento no es un directorio valido salta 
    error, en cualquier otro caso cambia al directorio dado*/
    else if (chdir(command[1])!=0)
        perror("Imposible cambiar directorio");
}

void f_date()
{
    //se guarda en el struct local los datos referentes al horario actual
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);

    //se asignan el anio, mes y día a las variables y se imprimen
    int y = local->tm_year+1900, m = local->tm_mon+1, d = local->tm_mday;

    printf("%02d/%02d/%d\n", d, m, y);
}

void f_time()
{
    //se guarda en el struct local los datos referentes al horario actual
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);

    //se asignan el anio, mes y día a las variables y se imprimen
    int h = local->tm_hour, m = local->tm_min, s = local->tm_sec;

    printf("%02d:%02d:%02d\n", h, m, s);
}

void f_hist(char ** command, tList * command_history)
{
    //si no hay argumentos se imprime el historial
    if (command[1]==NULL)
    {
        printHistUntil(listLength(*command_history), *command_history);
        return;
    }
    
    bool error = false;

    //si el argumento es -c se borra la lista
    if (strcmp(command[1], "-c")==0)
        deleteList(command_history);
    //si no hay que comprobar si lo que hay despues es un numero
    else if (command[1][0]=='-')
    {
        char* number = malloc(MAX_PROMPT * sizeof(char*));

        //se comprueba que hay memoria suficiente
        if (number==NULL)
        {
            perror("Error al asignar memoria");
            return;
        }

        //se copia en (string) number el digito (ejemplo, si el argumento es -34, se guarda 34)
        strncpy(number, command[1]+1, strlen(command[1]));

        //compruebo que number se corresponde con un entero positivo
        if (!isDigitString(number))
            error = true;

        /*si hay algun caracter distinto simplemente da error, si no se 
        imprime hasta el numero dado*/
        if (!error)
            printHistUntil(atoi(number), *command_history);

        free(number);
    }
}

void f_command(char ** command, tList * command_history, tList * open_files)
{
    //si no hay argumentos se imprime el historial
    if (command[1]==NULL)
    {
        printHistUntil(listLength(*command_history), *command_history);
        return;
    }

    //se comprueba que el primer argumento se corresponde con un entero positivo
    if (!isDigitString(command[1]))
        return;

    int goal_index = atoi(command[1]), current_index = 0;
    char *string = malloc(MAX_PROMPT*sizeof(char*)), **strings = malloc(MAX_PROMPT*sizeof(char*));

    //se comprueba que hay memoria suficiente
    if (string==NULL || strings == NULL)
    {
        printf("Error al asignar memoria");
        return;
    }

    tPosL i;
    //se recorre la lista de historial hasta el numero dado o nulo si no se encuentra
    for (i = first(*command_history); i!=LNULL && current_index<goal_index; i = next(i))
        ++current_index;

    //si no se encuentra da error
    if (i==NULL)
        printf("No hay elemento %s en el histórico\n", command[1]);
    //si no se ejecuta ese comando
    else
    {
        strcpy(string, getItem(i));
        printf("Ejecutando hist (%s): %s\n", command[1], string);
        TrocearCadena(string, strings);
        processCommand(strings, command_history, open_files);
    }

    free(string);
    free(strings);
}

void f_open(char ** command, tList * open_files)
{
    int i, df, mode = 0;
    char *mode_c = malloc(MAX_PROMPT * sizeof(char*)), *output = malloc(MAX_PROMPT * sizeof(char*));

    //se comprueba que hay memoria suficiente
    if (mode_c == NULL || output == NULL)
    {
        printf("Error al asignar memoria");
        return;
    }

    //si no hay argumentos se imprime la lista de archivos abiertos
    if (command[1]==NULL)
    {
        printOpenListByDF(*open_files);
        free(mode_c);
        free(output);
        return;
    }

    //se itera desde el segundo argumento asignando los valores correspondientes
    for (i=2; command[i]!=NULL; ++i)
    {
        if (!strcmp(command[i],"cr"))
        {
            mode|=O_CREAT;
            strcpy(mode_c, "O_CREAT");
        }
        else if (!strcmp(command[i],"ex"))
        {
            mode|=O_EXCL;
            strcpy(mode_c, "O_EXCL");
        }
        else if (!strcmp(command[i],"ro"))
        {
            mode|=O_RDONLY;
            strcpy(mode_c, "O_RDONLY");
        }
        else if (!strcmp(command[i],"wo"))
        {
            mode|=O_WRONLY;
            strcpy(mode_c, "O_WRONLY");
        }
        else if (!strcmp(command[i],"rw"))
        {
            mode|=O_RDWR;
            strcpy(mode_c, "O_RDWR");
        }
        else if (!strcmp(command[i],"ap"))
        {
            mode|=O_APPEND;
            strcpy(mode_c, "O_APPEND");
        }
        else if (!strcmp(command[i],"tr"))
        {
            mode|=O_TRUNC;
            strcpy(mode_c, "O_TRUNC");
        }
        else break;
    }

    //se intenta abrir y no se puede da error
    if ((df=open(command[1], mode, 0777))==-1)
        perror("Imposible abrir fichero");
    //si no simplemente se abrio y se añade a la lista de ficheros abiertos con su descriptor y modo
    else
    {
        sprintf(output, "Descriptor: %d -> %s %s", df, command[1], mode_c);
        insertItem(output, LNULL, open_files);
        printf("Anadida entrada %d a la tabla ficheros abiertos\n", df);
    }

    free(mode_c);
    free(output);
}

void f_close (char ** command, tList * open_files)
{
    int df;

    //si no hay argumentos se imprime la lista de archivos abiertos
    if (command[1]==NULL)
    {
        printOpenListByDF(*open_files);
        return;
    }

    //se comprueba que el primer argumento se corresponde con un entero positivo
    if (!isDigitString(command[1]))
        return;

    //se intenta cerrar y no se puede da error
    if (close(df = atoi(command[1]))==-1)
        perror("Imposible cerrar descriptor");
    //si no simplemente se cerro y se elimina de la lista de ficheros abiertos
    else
        deleteAtPosition(getPosByDF(df, *open_files), open_files);
}

//funcion completamente auxiliar para evitar la repeticion de codigo en esta funcion especifica
void free_all_dup(char * output, char * p, char * mode, char * aux, char ** aux_strings)
{
    free(output);
    free(p);
    free(mode);
    free(aux);
    free(aux_strings);
}
void f_dup(char ** command, tList * open_files)
{
    int df;
    char *output = malloc(MAX_PROMPT * sizeof(char*)), *p = malloc(MAX_PROMPT * sizeof(char*)), *mode = malloc(MAX_PROMPT * sizeof(char*)), *aux = malloc(MAX_PROMPT * sizeof(char*)), **aux_strings = malloc(MAX_PROMPT * sizeof(char*));
    strcpy(p, "");

    //se comprueba que hay memoria suficiente
    if (output == NULL || p == NULL || mode == NULL || aux == NULL || aux_strings == NULL)
    {
        free_all_dup(output, p, mode, aux, aux_strings);
        printf("Error al asignar memoria");
        return;
    }

    //si no hay argumentos se imprime la lista de archivos abiertos
    if (command[1]==NULL)
    {
        printOpenListByDF(*open_files);
        free_all_dup(output, p, mode, aux, aux_strings);
        return;
    }

    //se comprueba que el primer argumento se corresponde con un entero positivo y se guarda en df
    if (!isDigitString(command[1]))
    {
        free_all_dup(output, p, mode, aux, aux_strings);
        return;
    }
    df = atoi(command[1]);

    /*guardamos la posicion y si existe almacenamos el string en una variable auxiliar 
    para separarla en un array de cadenas y manipularlas mejor*/
    tPosL pos = getPosByDF(df, *open_files);
    if (pos!=LNULL)
        strcpy(aux, getItem(pos));
    else
    {
        printf("Imposible duplicar descriptor\n");
        free_all_dup(output, p, mode, aux, aux_strings);
        return;
    }
    TrocearCadena(aux, aux_strings);


    bool inMode = false;
    //se itera desde el modo comprobando si se tiene que añadir el string al string p o mode
    //p almacena lo que va dentro del paréntesis, el string desde el archivo hasta el modo sin incluirlo, y mode el modo
    for (int i = 3; aux_strings[i]!=NULL; ++i)
    {
        if ((!strcmp(aux_strings[i], "O_CREAT")
             || !strcmp(aux_strings[i], "O_EXCL")
             || !strcmp(aux_strings[i], "O_RDONLY")
             || !strcmp(aux_strings[i], "O_WRONLY")
             || !strcmp(aux_strings[i], "O_RDWR")
             || !strcmp(aux_strings[i], "O_APPEND")
             || !strcmp(aux_strings[i], "O_TRUNC"))
            && i!=3)
            inMode = true;

        if (!inMode)
        {
            strcat(p, aux_strings[i]);
            strcat(p, " ");
        }
        else
            strcpy(mode, aux_strings[i]);
    }
    //se quita el espacio sobrante
    p[strlen(p)-1] = '\0';

    //se coloca todo en su lugar y se inserta en la lista de archivos abiertos
    sprintf(output, "Descriptor: %d -> dup %d (%s) %s", dup(df), df, p, mode);
    insertItem(output, LNULL, open_files);

    free_all_dup(output, p, mode, aux, aux_strings);
}

void f_listopen(char ** command, tList open_files)
{
    //si no hay argumentos se imprime la lista de archivos abiertos
    if (command[1]==NULL)
        printOpenListByDF(open_files);
    else
    {
        /*se comprueba que el primer argumento se corresponde con un entero positivo,
        si lo es se imprime la lista de arhivos abiertos hasta ese numero*/
        if (!isDigitString(command[1]))
            return;
        printOpenListByDFUntil(atoi(command[1]), open_files);
    }
}

void f_infosys()
{
    //se guardan los datos del dispositivo en un struct y se imprimen
    struct utsname unameData;
    uname(&unameData);
    printf("%s (%s), OS: %s-%s-%s\n", unameData.nodename, unameData.machine, unameData.sysname, unameData.release, unameData.version);
}

void f_help(char ** command)
{
    //se muestra la ayuda correspondiente a cada comando
    if (command[1]==NULL)
        printf("'help [cmd|-lt|-T topic]' ayuda sobre comandos\n\t\tComandos disponibles:\nauthors pid chdir date time hist command open close dup listopen infosys help quit exit bye\n");
    else if (!strcmp(command[1], "authors"))
        printf("authors [-n|-l]: Muestra los nombres y/o logins de los autores\n");
    else if (!strcmp(command[1], "pid"))
        printf("pid [-p]: Muestra el pid del shell o de su proceso padre\n");
    else if (!strcmp(command[1], "chdir"))
        printf("chdir [dir]: Cambia (o muestra) el directorio actual del shell\n");
    else if (!strcmp(command[1], "date"))
        printf("date: Muestra la fecha actual\n");
    else if (!strcmp(command[1], "time"))
        printf("time: Muestra la hora actual\n");
    else if (!strcmp(command[1], "hist"))
        printf("hist [-c|-N]: Muestra (o borra) el historico de comandos\n\t-N: muestra los N primeros\n\t-c: borra el historico\n");
    else if (!strcmp(command[1], "command"))
        printf("command [-N]: Repite el comando N (del historico)\n");
    else if (!strcmp(command[1], "open"))
        printf("open fich m1 m2...:	Abre el fichero fich. y lo anade a la lista de ficheros abiertos del shell\nm1, m2..es el modo de apertura (or bit a bit de los siguientes)\n\tcr: O_CREAT\tap: O_APPEND\n\tex: O_EXCL\tro: O_RDONLY\n\trw: O_RDWR\two: O_WRONLY\n\ttr: O_TRUNC\n");
    else if (!strcmp(command[1], "close"))
        printf("close df: Cierra el descriptor df y elimina el correspondiente fichero de la lista de ficheros abiertos\n");
    else if (!strcmp(command[1], "dup"))
        printf("dup df: Duplica el descriptor de fichero df y anade una nueva entrada a la lista ficheros abiertos\n");
    else if (!strcmp(command[1], "listopen"))
        printf("listopen [n]: Lista los ficheros abiertos (al menos n) del shell\n");
    else if (!strcmp(command[1], "infosys"))
        printf("infosys: Muestra informacion de la maquina donde corre el shell\n");
    else if (!strcmp(command[1], "help"))
        printf("help [cmd|-lt|-T]: Muestra ayuda sobre los comandos\n\t-lt: lista topics de ayuda\n\t-T topic: lista comandos sobre ese topic\n\tcmd: info sobre el comando cmd\n");
    else if (!strcmp(command[1], "quit") || !strcmp(command[1], "exit") || !strcmp(command[1], "bye"))
        printf("%s: Termina la ejecucion del shell\n", command[1]);
    else
        printf("%s no encontrado\n", command[1]);
}

void f_quit(tList * command_history, tList * open_files)
{
    //se libera la memoria de las listas y se termina el programa;
    deleteList(command_history);
    deleteList(open_files);
    exit(0);
}

void f_invalid()
{
    //si el comando no es valido salta error
    printf("No ejecutado: No such file or directory\n");
}

void f_create(char ** command)
{
    if (command[1]==NULL)
        printCurrentDir();
    else if (mkdir(command[1], 0777)!=0)
        perror("Imposible crear directorio");
}

void f_stat(char ** command)
{
    
}

void f_list(char ** command)
{
    if (command[1]==NULL)
    {
        printCurrentDir();
        return;
    }

    DIR *dir = opendir(command[1]);

    if (dir == NULL) 
    {
        perror("Error al abrir el directorio");
        return;
    }

    struct stat fileInfo;
    struct dirent *currentFile;

    while ((currentFile = readdir(dir)) != NULL && stat(currentFile->d_name, &fileInfo) == 0) 
    {
        printf("%lld  %s\n",(long long)fileInfo.st_size, currentFile->d_name);
    }

    closedir(dir)
    return 0;
}



void processCommand(char ** command, tList * command_history, tList * open_files)
{
    //se ejecuta el comando correspondiente a command
    if (command[0]==NULL)
        return;
    if (!strcmp(command[0], "authors"))
        f_authors(command);
    else if (!strcmp(command[0], "pid"))
        f_pid(command);
    else if (!strcmp(command[0], "chdir"))
        f_chdir(command);
    else if (!strcmp(command[0], "date"))
        f_date();
    else if (!strcmp(command[0], "time"))
        f_time();
    else if (!strcmp(command[0], "hist"))
        f_hist(command, command_history);
    else if (!strcmp(command[0], "command"))
        f_command(command, command_history, open_files);
    else if (!strcmp(command[0], "open"))
        f_open(command, open_files);
    else if (!strcmp(command[0], "close"))
        f_close(command, open_files);
    else if (!strcmp(command[0], "dup"))
        f_dup(command, open_files);
    else if (!strcmp(command[0], "listopen"))
        f_listopen(command, *open_files);
    else if (!strcmp(command[0], "infosys"))
        f_infosys();
    else if (!strcmp(command[0], "help"))
        f_help(command);
    else if (!strcmp(command[0], "quit") || !strcmp(command[0], "exit") || !strcmp(command[0], "bye"))
        f_quit(command_history, open_files);
    else if (!strcmp(command[0], "clear"))
        system("clear");
    else if (!strcmp(command[0], "create"))
        f_create(command);
    else if (!strcmp(command[0], "stat"))
        f_stat(command);
    else if (!strcmp(command[0], "list"))
        return;
    else if (!strcmp(command[0], "delete"))
        return;
    else if (!strcmp(command[0], "deltree"))
        return;
    else
        f_invalid();
}