//
// Created by otavio on 24/10/2019.
//

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

char* utilsGetLine();

void clearInput(char* linha, List this);

int isGhostCommand(char* command);

void generateGhostSon();

char** createMatrix(char* command, char** matrix);

int myWait();

int cleanAndDie();

void sigIntHandler(int sig);

#endif //_GHOSTSHELL_UTILS_H
