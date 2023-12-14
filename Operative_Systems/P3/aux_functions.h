#ifndef AUX_FUNCTIONS_H
#define AUX_FUNCTIONS_H

#include "list.h"
#include "functions.h"

void freeAll(int n, ...);
void freeAllRec(int n, ...);
int trimString(char * string, char ** strings);
void untrimString(char ** strings, char * string);
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
void show_credentials();
int findVariable(char * var, char ** e);
int changeVar(char * var, char * valor, char ** e);
int stringsSize(char ** strings);
char *processStatus(char c);
//tPosL getPosByPid(int pid, tList *bg_proc);
int signalValue(char * signal);
const char *signalName(int signal);
int getPidFromPos(tPosL pos);


#endif //AUX_FUNCTIONS_H