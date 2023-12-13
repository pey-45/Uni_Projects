#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/resource.h>

#define MAX_PROMPT 16384
#define MALLOC_PTR malloc(MAX_PROMPT*sizeof(char*))
#define MALLOC malloc(MAX_PROMPT*sizeof(char))

typedef char tItemL[MAX_PROMPT];
typedef struct tNode* tPosL;
struct tNode 
{
    tItemL data;
    tPosL next;
};
typedef tPosL tList;
bool isEmptyList(tList L);
void createEmptyList(tList *L);
bool createNode(tPosL *p);
bool insertItem(tItemL d, tPosL p, tList *L);
void updateItem(tItemL d, tPosL p);
tPosL findItem(tItemL d, tList L);
char* getItem(tPosL pos);
tPosL first(tList L);
tPosL last(tList L);
tPosL previous(tPosL p, tList L);
tPosL next(tPosL p);
void deleteAtPosition(tPosL p, tList *L);
void deleteList(tList *L);
bool copyList(tList L, tList *M);

//only for this project
int listLength(tList L);
void printItem(tPosL p);

#endif //LIST_H