#include "functions.h"

void f_authors(char ** command)
{
    //si no tiene argumentos imprime los nombres y logins
    if (!command[1]) printf("Pablo Manzanares Lopez: pablo.manzanares.lopez@udc.es\nAlejandro Rodriguez Franco: alejandro.rodriguezf@udc.es\n");
    //si tiene de argumento -l imprime los logins
    else if (!strcmp(command[1], "-l")) printf("pablo.manzanares.lopez@udc.es\nalejandro.rodriguezf@udc.es\n");
    //si tiene de argumento -n imprime los nombres
    else if (!strcmp(command[1], "-n")) printf("Pablo Manzanares Lopez\nAlejandro Rodriguez Franco\n");
    //si no no imprime nada
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
    /*si el argumento no es un directorio valido salta 
    error, en cualquier otro caso cambia al directorio dado*/
    else if (chdir(command[1])!=0) perror("Imposible cambiar directorio");
}

void f_date()
{
    //se guarda en el struct local los datos referentes al horario actual
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);

    //se imprimen el anio, mes y día 
    printf("%02d/%02d/%d\n", local->tm_mday, local->tm_mon+1, local->tm_year+1900);
}

void f_time()
{
    //se guarda en el struct local los datos referentes al horario actual
    time_t t;
    time(&t);
    struct tm *local = localtime(&t);

    //se imprimen la hora, minutos y segundos
    printf("%02d:%02d:%02d\n", local->tm_hour, local->tm_min, local->tm_sec);
}

void f_hist(char ** command, tList * command_history)
{
    //si no hay argumentos se imprime el historial
    if (!command[1])
    {
        printHistUntil(listLength(*command_history), *command_history);
        return;
    }
    
    //si el argumento es -c se borra la lista
    if (!strcmp(command[1], "-c")) deleteList(command_history);
    //si no hay que comprobar si lo que hay despues es un numero
    else if (command[1][0]=='-' && isDigitString(command[1]+1 /*sin el guion*/)) printHistUntil(atoi(command[1]+1), *command_history);
}

void f_command(char ** command, tList * command_history, tList * open_files) 
{
    char *string = MALLOC, **strings = MALLOC_PTR;
    int goal_index, current_index = 0;
    tPosL i;

    //si no hay argumentos se imprime el historial
    if (!command[1] || !isDigitString(command[1]) || !string || !strings) 
    {
        if (!command[1]) printHistUntil(listLength(*command_history), *command_history);
        else if (isDigitString(command[1])) printf("Error al asignar memoria");
        freeStrings(1, string);
        freeMatrix(1, strings);
        return;
    }

    goal_index = atoi(command[1]);

    //se recorre la lista de historial hasta el numero dado o nulo si no se encuentra
    for (i = first(*command_history); i && current_index++ < goal_index; i = next(i));

    //si no se encuentra da error
    if (!i) printf("No hay elemento %s en el histórico\n", command[1]);
    //si no se ejecuta ese comando
    else 
    {
        strcpy(string, getItem(i));
        printf("Ejecutando hist (%s): %s\n", command[1], string);
        TrocearCadena(string, strings);
        processCommand(strings, command_history, open_files);
    }

    freeStrings(1, string);
    freeMatrix(1, strings);
}

void f_open(char ** command, tList * open_files, bool show_message)
{
    int i, df, mode = 0;
    char *mode_c = MALLOC, *output = MALLOC;

    //se comprueba que hay memoria suficiente
    if (!command[1] || !mode_c || !output)
    {
        !command[1]? printOpenListByDF(*open_files):printf("Error al asignar memoria");
        freeStrings(2, mode_c, output);
        return;
    }
    //para evitar saltos condicionales dependientes de variables sin inicializar
    initializeString(mode_c);

    //se itera desde el segundo argumento asignando los valores correspondientes
    for (i=2; command[i]; ++i)
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
    if ((df=open(command[1], mode, 0777))==-1) perror("Imposible abrir fichero");
    //si no simplemente se abrio y se añade a la lista de ficheros abiertos con su descriptor y modo
    else
    {
        sprintf(output, "Descriptor: %d -> %s %s", df, command[1], mode_c);
        insertItem(output, NULL, open_files);
        if (show_message) printf("Anadida entrada %d a la tabla ficheros abiertos\n", df);
    }

    freeStrings(2, mode_c, output);
}

void f_close (char ** command, tList * open_files)
{
    int df;

    //si no hay argumentos se imprime la lista de archivos abiertos
    if (!command[1])
    {
        printOpenListByDF(*open_files);
        return;
    }

    //se comprueba que el primer argumento se corresponde con un entero positivo
    if (!isDigitString(command[1])) return;

    //se intenta cerrar y no se puede da error
    if (close(df = atoi(command[1]))==-1) perror("Imposible cerrar descriptor");
    //si no simplemente se cerro y se elimina de la lista de ficheros abiertos
    else deleteAtPosition(getPosByDF(df, *open_files), open_files);
}

void f_dup(char ** command, tList * open_files)
{
    int df;
    char *output = MALLOC, *p = MALLOC, *mode = MALLOC, *aux = MALLOC, **aux_strings = MALLOC_PTR;
    strcpy(p, "");

    if (!output || !p || !mode || !aux || !aux_strings || !command[1] || !isDigitString(command[1]))
    {
        //si no hay argumentos se imprime la lista de archivos abiertos
        if (!command[1]) printOpenListByDF(*open_files);
        //si es un entero positivo, por descarte el fallo es de asignacion de memoria
        else if (isDigitString(command[1])) perror("Error al asignar memoria");

        freeStrings(4, output, p, mode, aux);
        freeMatrix(1, aux_strings);
        return;
    }
    //el entero positivo se guarda en df
    df = atoi(command[1]);

    /*guardamos la posicion y si existe almacenamos el string en una variable auxiliar 
    para separarla en un array de cadenas y manipularlas mejor*/
    tPosL pos = getPosByDF(df, *open_files);
    if (pos) strcpy(aux, getItem(pos));
    else
    {
        perror("Imposible duplicar descriptor\n");
        freeStrings(4, output, p, mode, aux);
        freeMatrix(1, aux_strings);
        return;
    }

    TrocearCadena(aux, aux_strings);

    bool inMode = false;
    //se itera desde el modo comprobando si se tiene que añadir el string al string p o mode
    //p almacena lo que va dentro del paréntesis, el string desde el archivo hasta el modo sin incluirlo, y mode el modo
    for (int i = 3; aux_strings[i]; ++i)
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
        else strcpy(mode, aux_strings[i]);
    }
    //se quita el espacio sobrante
    p[strlen(p)-1] = '\0';

    //se coloca tod0 en su lugar y se inserta en la lista de archivos abiertos
    sprintf(output, "Descriptor: %d -> dup %d (%s) %s", dup(df), df, p, mode);
    insertItem(output, NULL, open_files);

    freeStrings(4, output, p, mode, aux);
    freeMatrix(1, aux_strings);
}

void f_listopen(char ** command, tList open_files)
{
    //si no hay argumentos se imprime la lista de archivos abiertos
    if (!command[1]) printOpenListByDF(open_files);
    else
    {
        /*se comprueba que el primer argumento se corresponde con un entero positivo,
        si lo es se imprime la lista de arhivos abiertos hasta ese numero*/
        if (!isDigitString(command[1])) return;
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
    else printf("%s no encontrado\n", command[1]);
}

void f_quit(tList * command_history, tList * open_files)
{
    //se libera la memoria de las listas y se termina el programa;
    deleteList(command_history);
    deleteList(open_files);
    exit(0);
}

void f_invalid() { perror("No ejecutado"); }

void f_create(char ** command, tList * open_files)
{
    char *string = MALLOC, **strings = MALLOC_PTR;
    if (!string || !strings)
    {
        printf("Error al asignar memoria");
        freeStrings(1, string);
        freeMatrix(1, strings);
        return;
    }

    if (!command[1] || (!strcmp(command[1], "-f") && !command[2])) printCurrentDir();
    else if (!strcmp(command[1], "-f"))
    {
        sprintf(string, "open %s cr", command[2]);
        TrocearCadena(string, strings);
        f_open(strings, open_files, false);
    }
    else if (mkdir(command[1], 0777)!=0) perror("Imposible crear directorio");
    
    freeStrings(1, string);
    freeMatrix(1, strings);
}

void f_stat(char ** command)
{
    struct stat attr;
    int i;
    //reservo memoria para las listas de archivos y argumentos
    char **files = MALLOC_PTR, **args = MALLOC_PTR;
    if (!files || !args)
    {
        printf("Error al asignar memoria");
        freeMatrix(2, files, args);
        return;
    }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++)
    {
        files[i] = NULL;
        args[i] = NULL;
    }

    bool in_files = 0;
    int args_pos = 0, files_pos = 0;

    for (i = 1; command[i]; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long")!=0 && strcmp(command[i], "-link")!=0 && strcmp(command[i], "-acc")!=0) in_files = 1;

        if (!in_files)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[args_pos] = MALLOC;
            if (!args[args_pos])
            {
                perror("Error al asignar memoria");
                freeMatrixAllElements(2, files, args);
                return;
            }
            args[args_pos][0] = '\0'; //para evitar fallos con el strcmp

            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[args_pos++], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[args_pos++], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[args_pos++], "acc");
            else
            {
                //si se repite un parámetro se libera la posicion ya que no se añadirá nada, tampoco incrementa breakpoint
                freeStrings(1, args[args_pos]);
                args[args_pos] = NULL;
            }
        }       
        else
        {
            files[files_pos] = MALLOC;
            if (!files[files_pos])
            {
                perror("Error al asignar memoria");
                freeMatrixAllElements(2, files, args);
                return;
            }
            strcpy(files[files_pos++], command[i]);
        }
    }

    if (!files[0])
    {
        printCurrentDir();
        freeMatrixAllElements(2, files, args);
        return;
    }

    if (includesString("long", args))
    {
        for (i = 0; i < files_pos; i++)
        {
            if (!stat(files[i], &attr)) printStat(files[i], attr, "long", includesString("link", args), false);
            else fprintf(stderr, "Error al acceder a %s: %s\n", files[i], strerror(errno));
        }
    }
    else if (includesString("acc", args))
    {
        for (i = 0; i < files_pos; i++)
        {
            if (!stat(files[i], &attr)) printStat(files[i], attr, "acc", includesString("link", args), false);
            else fprintf(stderr, "Error al acceder a %s: %s\n", files[i], strerror(errno));
        }
    }
    else if (includesString("link", args))
    {
        for (i = 0; i < files_pos; i++)
        {
            if (!stat(files[i], &attr)) printStat(files[i], attr, "link", true, false);
            else fprintf(stderr, "Error al acceder a %s: %s\n", files[i], strerror(errno));
        }
    }
    else
    {
        for (i = 0; i < files_pos; i++)
        {
            if (!stat(files[i], &attr)) printStat(files[i], attr, "few", includesString("link", args), false);
            else fprintf(stderr, "Error al acceder a %s: %s\n", files[i], strerror(errno));
        }
    }

    freeMatrixAllElements(2, files, args);
}

void f_list(char ** command)
{
    if (!command[1])
    {
        printCurrentDir();
        return;
    } 

    int i, args_pos = 0, dirs_pos = 0;
    char **dirs = MALLOC_PTR, **args = MALLOC_PTR;
    char recAorB = '\0';
    bool hid = false;
    bool in_dirs = 0;

    if (!dirs || !args)
    {
        perror("Error al asignar memoria");
        freeMatrix(2, dirs, args);
        return;
    }

    //inicializo sus elementos como nulos
    for (i = 0; i < MAX_PROMPT; i++)
    {
        dirs[i] = NULL;
        args[i] = NULL;
    }

    for (i = 1; command[i]; i++)
    {
        //si el comando no es ningun argumento válido tod0 lo que haya delante sera considerado un archivo
        if (strcmp(command[i], "-long")!=0 && strcmp(command[i], "-link")!=0 && strcmp(command[i], "-acc")!=0 && strcmp(command[i], "-reca")!=0 
            && strcmp(command[i], "-recb")!=0 && strcmp(command[i], "-hid")!=0) in_dirs = 1;

        if (!in_dirs)
        {
            //se inicializa la posicion en la que se guardará el argumento
            args[args_pos] = MALLOC;
            args[args_pos][0] = '\0';
            if (!args[args_pos])
            {
                perror("Error al asignar memoria");
                freeMatrixAllElements(2, dirs, args);
                return;
            }
            //se guarda cuales de los parametros se han pasado
            if (!strcmp(command[i], "-long") && !includesString("long", args)) strcpy(args[args_pos], "long");
            else if (!strcmp(command[i], "-link") && !includesString("link", args)) strcpy(args[args_pos], "link");
            else if (!strcmp(command[i], "-acc") && !includesString("acc", args)) strcpy(args[args_pos], "acc");
            else if (!strcmp(command[i], "-reca")) recAorB = 'A';
            else if (!strcmp(command[i], "-recb")) recAorB = 'B';
            else if (!strcmp(command[i], "-hid")) hid = true;
            else
            {
                //si se repite un parámetro entre long, acc y link se libera la posicion ya que no se añadirá nada, tampoco incrementa breakpoint
                freeStrings(1, args[args_pos]);
                args[args_pos] = NULL;
                args_pos --;
            }
            args_pos++;
        }       
        else
        {
            dirs[dirs_pos] = MALLOC;
            if (!dirs[dirs_pos])
            {
                perror("Error al asignar memoria");
                freeMatrixAllElements(2, dirs, args);
                return;
            }
            strcpy(dirs[dirs_pos++], command[i]);
        }
    }

    if (!dirs[0])
    {
        printCurrentDir();
        freeMatrixAllElements(2, dirs, args);
        return;
    }

    if (recAorB == 'B') for (i = 0; i < dirs_pos; i++) listDirElements(dirs[i], args, recAorB, hid, false);
    else if (recAorB == 'A') for (i = 0; i < dirs_pos; i++) listDirElements(dirs[i], args, recAorB, hid, false);
    else for (i = 0; i < dirs_pos; i++) listDirElements(dirs[i], args, '\0', hid, false);
    

    freeMatrixAllElements(2, dirs, args);
}

void f_delete(char ** command)
{
    struct stat attr;
    char sure, aux;
    int i;

    if (!command[1]) 
    {
        printCurrentDir();
        return;
    }

    if (!strcmp(command[0], "deltree"))
    {
        printf("Esta es una acción destructiva, deseas continuar [y/n]? ");
        scanf("%c", &sure);
        scanf("%c", &aux);
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

void processCommand(char ** command, tList * command_history, tList * open_files)
{
    //se ejecuta el comando correspondiente a command
    if (!command[0]) return;
    if (!strcmp(command[0], "authors")) f_authors(command);
    else if (!strcmp(command[0], "pid")) f_pid(command);
    else if (!strcmp(command[0], "chdir")) f_chdir(command);
    else if (!strcmp(command[0], "date")) f_date();
    else if (!strcmp(command[0], "time")) f_time();
    else if (!strcmp(command[0], "hist")) f_hist(command, command_history);
    else if (!strcmp(command[0], "command")) f_command(command, command_history, open_files);
    else if (!strcmp(command[0], "open")) f_open(command, open_files, true);
    else if (!strcmp(command[0], "close")) f_close(command, open_files);
    else if (!strcmp(command[0], "dup")) f_dup(command, open_files);
    else if (!strcmp(command[0], "listopen")) f_listopen(command, *open_files);
    else if (!strcmp(command[0], "infosys")) f_infosys();
    else if (!strcmp(command[0], "help")) f_help(command);
    else if (!strcmp(command[0], "quit") || !strcmp(command[0], "exit") || !strcmp(command[0], "bye")) f_quit(command_history, open_files);
    else if (!strcmp(command[0], "clear")) system("clear");
    else if (!strcmp(command[0], "create")) f_create(command, open_files);
    else if (!strcmp(command[0], "stat")) f_stat(command);
    else if (!strcmp(command[0], "list")) f_list(command);
    else if (!strcmp(command[0], "delete") || !strcmp(command[0], "deltree")) f_delete(command);
    else f_invalid();
}