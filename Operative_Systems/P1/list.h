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

#define LNULL NULL
#define MAX_PROMPT 16384

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