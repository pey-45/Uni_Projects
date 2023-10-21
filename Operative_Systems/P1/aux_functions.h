#include "list.h"

void freeStrings(int n, ...);
void freeMatrix(int n, ...);
int TrocearCadena(char * cadena, char * trozos[]);
tPosL getPosByDF(int df, tList L);
void printOpenListByDF(tList L);
void printOpenListByDFUntil(int limit, tList L);
void printHistUntil(int limit, tList L);
bool isDigitString(char * string);
void printCurrentDir();
char LetraTF (mode_t m);
void printLong(char * file, struct stat attr);
void printLink(const char * file);
void printAcc(const char * file, struct stat attr);
void printFew(const char * file, struct stat attr);
char * ConvierteModo (mode_t m, char * permisos);
bool includesString(char * string, char ** strings);
void printAsStat(char * dir, char ** args, struct dirent *entry);
void printDirElements(const char *_dir, struct dirent *entry, char ** args, bool hid);
void printDirElementsRB(const char *_dir, struct dirent *entry, char ** args, bool hid);
void printDirElementsRA(const char *_dir, struct dirent *entry, char ** args, bool hid);
int getPos(char * string, char ** strings);