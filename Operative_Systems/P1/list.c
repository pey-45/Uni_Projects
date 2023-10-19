#include "list.h"

bool isEmptyList(tList L) { return (L == LNULL); }

void createEmptyList(tList *L) { *L = LNULL; }

bool createNode(tPosL *p)
{
    *p = malloc(sizeof(struct tNode));
    return *p != LNULL;
}

bool insertItem(tItemL d, tPosL p, tList *L)
{
    tPosL q, r;
    if (!createNode(&q))return false;
    else
    {
        strcpy(q->data, d);
        q->next = LNULL;
        if (isEmptyList(*L))
            *L = q;
        else if (p == LNULL)
        {
            for (r = *L; r->next != LNULL; r = r->next);
            r->next = q;
        }
        else if (p == *L)
        {
            q->next = p;
            *L = q;
        }
        else
        {
            strcpy(q->data, p->data);
            strcpy(p->data, d);
            q->next = p->next;
            p->next = q;
        }
        return true;
    }
}

void updateItem(tItemL d, tPosL p) { strcpy(p->data, d); }

tPosL findItem(tItemL d, tList L)
{
    tPosL p;

    for (p = L; (p != LNULL) && (p->data != d); p = p->next);
    return p;
}

char* getItem(tPosL pos) { return pos->data; }

tPosL first(tList L) { return L; }

tPosL last(tList L)
{
    tPosL p;

    for (p = L; p->next != LNULL; p = p->next);
    return p;
}

tPosL previous(tPosL p, tList L)
{
    tPosL q;

    if (p == L) return LNULL;
    else
    {
        for (q = L; q->next != p; q = q->next);
        return q;
    }
}

tPosL next(tPosL p) {  return p->next; }

void deleteAtPosition(tPosL p, tList *L)
{
    tPosL q;

    if (p == *L)
    {
        *L = (*L)->next;
    }
    else if (p->next == LNULL)
    {
        for (q = *L; q->next->next != LNULL; q = q->next);
        q->next = LNULL;
    }
    else
    {
        q = p->next;
        strcpy(p->data, q->data);
        p->next = q->next;
        p = q;
    }

    free(p);
    p = NULL;
}

void deleteList(tList *L)
{
    tPosL p;

    while (!isEmptyList(*L))
    {
        p = *L;
        *L = (*L)->next;
        free(p);
        p = NULL;
    }
}

bool copyList(tList L, tList *M)
{
    tPosL p, q, r;
    bool result = true;

    createEmptyList(M);
    if (!isEmptyList(L))
    {
        p = L;
        while ((p != LNULL) && (createNode(&r)))
        {
            strcpy(r->data, p->data);
            r->next = LNULL;

            if (p == L)
            {
                *M = r;
                q = r;
            }
            else
            {
                q->next = r;
                q = r;
            }
            p = p->next;
        }

        if (p != LNULL)
        {
            result = false;
            deleteList(M);
        }
    }

    return result;
}


int listLength(tList L)
{
  	int index = 1;
  	tPosL i;
  	for (i = L; i->next!=LNULL; i = i->next)
		++index;
  	return i!=LNULL? index:0;
}

void printItem(tPosL p) 
{ 
	if (p!=LNULL)
    printf("%s", p->data);
}