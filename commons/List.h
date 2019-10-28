//
// Created by otavio on 22/10/2019.
//
//implementacao de uma lista generica
#ifndef _GHOSTSHELL_LIST_H
#define _GHOSTSHELL_LIST_H

#include <stdlib.h>
#include <stdio.h>
#define DYNAMIC 1
#define STATIC 0


typedef struct list_* List;

//funcao para criar a lista
List listNew(unsigned int isDynamic);

//funcao para pegar um elemento na lista com bsae em um indice
void* listGet(List this, unsigned int index);

//libera a memoria alocada para a lista
void listFree(List this);

//adiciona um elemento ao final da lista
void listAdd(List this, void* item);

//remove um elmento da lista baseado em um indice
void* listRemove(List this, unsigned int index);

//retorna o comprimento da lista
int listGetLength(List this);

//esssa funcao deve ser chamada antes de percorrer a lista com listNext
void listRestart(List this);

//listNext usa para retornar o proximo elemento da lista
void* listNext(List this);

//libera a memoria alocada para a lista e aloca uma nova lista
void clean(List this);

// remove um elemento da lista baseado no pid
void* listRemoveByPid(List this, pid_t pid);

#endif //_GHOSTSHELL_LIST_H
