//Pablo Manzanares López      ---  pablo.manzanares.lopez@udc.es
//Alejandro Rodríguez Franco  ---  alejandro.rodriguezf@udc.es

#include "functions.h"

int main()
{
	char command[MAX_PROMPT], *full_command[MAX_PROMPT];

	//se crean las listas de historial y archivos abiertos
	tList command_history;
	tList open_files;
	createEmptyList(&command_history);
	createEmptyList(&open_files);

	//se añaden los elementos por defecto a la lista de archivos abiertos
	insertItem("Descriptor: 0 -> entrada estandar O_RDWR", LNULL, &open_files);
	insertItem("Descriptor: 1 -> salida estandar O_RDWR", LNULL, &open_files);
	insertItem("Descriptor: 2 -> error estandar O_RDWR", LNULL, &open_files);
	
	while (true)
	{
		printf("-> ");
		//se guarda en command el comando escrito
		fgets(command, MAX_PROMPT, stdin);

		//si no es un salto de línea
		if (strcmp(command, "\n")!=0)
		{
			//se elimina el salto de linea
			command[strlen(command)-1] = '\0';
			//se inserta el comando en el historial de comandos
			insertItem(command, LNULL, &command_history);
			//se trocea la cadena y se procesa como array de cadenas
			TrocearCadena(command, full_command);
			processCommand(full_command, &command_history, &open_files);
		}
	}
}
