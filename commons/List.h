//
// Created by otavio on 22/10/2019.
//

#ifndef _GHOSTSHELL_LIST_H
#define _GHOSTSHELL_LIST_H

#include <stdlib.h>
#include <stdio.h>
#define DYNAMIC 1
#define STATIC 0


typedef struct list_* List;

List listNew();

void* listGet(List this, unsigned int index);

void listFree(List this);

void listAdd(List this, void* item);

void* listRemove(List this, unsigned int index);

int listGetLength(List this);

void listRestart(List this);

void* listNext(List this);

char** list2Matrix(List this);

#endif //_GHOSTSHELL_LIST_H
