#ifndef AUX_FUNCTIONS_H
#define AUX_FUNCTIONS_H

#include "list.h"
#include "functions.h"

void freeAll(int n, ...);
void freeAllRec(int n, ...);
int TrocearCadena(char * cadena, char ** trozos);
tPosL getPosByDF(int df, tList L);
tPosL getLastPosByDF(tList L);
void printOpenListByDF(tList L);
void printOpenListByDFUntil(int limit, tList L);
void printHistUntil(int limit, tList L);
bool isDigitString(char * string);
void printCurrentDir();
char LetraTF (mode_t m);
void printStat(char * file, struct stat attr, char * print_mode, bool link, bool is_from_list);
char *ConvierteModo (mode_t m, char * permisos);
bool includesString(char * string, char ** strings);
void listDirElements(char *_dir,  char ** args, char mode, bool hid, bool deltree);
char *getLastNamePath(char * dir);
void initializeString(char * string);
bool isDir(char * _dir);
void printList(tList * L);
void *getShm (key_t clave, size_t tam);
void *mmap_file(char * fichero, int protection, tList * mmap_memory);
ssize_t writeFile(char *f, void *p, size_t cont,int overwrite);
ssize_t readFile(char *f, void *p, size_t cont);
void doMemPmap();
void recurse(int n);



#endif //AUX_FUNCTIONS_H