//
// Created by otavio on 24/10/2019.
//
//Esse arquivo possui todas as funcoes necessarias para a shell

#ifndef _GHOSTSHELL_UTILS_H
#define _GHOSTSHELL_UTILS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "unistd.h"
#include "time.h"
#include "List.h"

//le uma linha da entrada padrao e aloca uma string
char* utilsGetLine();

//pega uma string e retorna uma lista de strings quebrando a string nos pontos onde exista um #
void clearInput(char* linha, List this);

//dado uma string retorna 1 se for igual a myWait, e retorna 2 se for igual a clean&die
int isGhostCommand(char* command);

//funcao chamada por um processo filho da shell para gerar um processo filho ghost
void generateGhostSon();

//dado uma string e um vetor de strings previamente alocados retorna um vetor de strings com os elementos da string comando original quebrandos aonde haviam espacos
char** createMatrix(char* command, char** matrix);

//implementacao do comando myait
int myWait();

//imlementacao do comando clean&die
int cleanAndDie();

//handler para o sinal SIGINT
void sigIntHandler(int sig);

//handler para o sinal SIGCHLD
void sigChildHandler(int sig);

//handler para o sinal SIGTSTP
void sigStopHandler(int sig);

//usado para limpar o buffer
void clearBuffer();

#endif //_GHOSTSHELL_UTILS_H
