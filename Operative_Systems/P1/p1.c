//Pablo Manzanares López      ---  pablo.manzanares.lopez@udc.es
//Alejandro Rodríguez Franco  ---  alejandro.rodriguezf@udc.es

#include "functions.h"
#define ever ;;

int main()
{
	char command[MAX_PROMPT], *full_command[MAX_PROMPT], *username = getenv("USER"), dir[MAX_PROMPT];

	//se crean las listas de historial y archivos abiertos
	tList command_history;
	tList open_files;
	createEmptyList(&command_history);
	createEmptyList(&open_files);

	//se añaden los elementos por defecto a la lista de archivos abiertos
	insertItem("Descriptor: 0 -> entrada estandar O_RDWR", NULL, &open_files);
	insertItem("Descriptor: 1 -> salida estandar O_RDWR", NULL, &open_files);
	insertItem("Descriptor: 2 -> error estandar O_RDWR", NULL, &open_files);
	
	//obtencion de nombre de usuario y nodename para formato de input
	if (!username)
	{
    	perror("No se pudo obtener el nombre de usuario.\n");
		f_quit(&command_history, &open_files);
	}
	struct utsname unameData;
	uname(&unameData);

	for(ever)
	{
		//obtencion de directorio actual para formato de input
		if (!getcwd(dir, MAX_PROMPT))
		{
    		perror("No se pudo obtener el directorio actual.\n");
			f_quit(&command_history, &open_files);
		}

		printf("\033[1;34m%s@%s\033[1;37m:\033[1;33m%s\033[0m$ ", username, unameData.nodename, dir);
		//se guarda en command el comando escrito
		fgets(command, MAX_PROMPT, stdin);

		//si no es un salto de línea
		if (strcmp(command, "\n")!=0)
		{
			//se elimina el salto de linea
			command[strlen(command)-1] = '\0';
			//se inserta el comando en el historial de comandos
			insertItem(command, NULL, &command_history);
			//se trocea la cadena y se procesa como array de cadenas
			TrocearCadena(command, full_command);
			processCommand(full_command, &command_history, &open_files);
		}
	}
}