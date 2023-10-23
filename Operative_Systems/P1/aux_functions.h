#include "list.h"

void freeStrings(int n, ...);
void freeMatrixAllElements(int n, ...);
void freeMatrix(int n, ...);
int TrocearCadena(char * cadena, char ** trozos);
tPosL getPosByDF(int df, tList L);
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
bool isDir(char * dir);