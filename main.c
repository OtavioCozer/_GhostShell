#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commons/List.h"
#include "commons/utils.h"



List pidList;
List pgidList;
List comandos;
int ignoreRead;

int main() {
    ignoreRead = 0;
    int (*oldSigChildHandler)();
    int (*oldSigStopHandler)();
    signal(SIGINT, sigIntHandler);
    oldSigChildHandler = signal(SIGCHLD, sigChildHandler);
    oldSigStopHandler = signal(20, sigStopHandler);
    printf("%d\n", getpid());

    int initialCommandSize = 0;
    char* line;
    pid_t pgid = 0;
    pid_t pid = 0;
    pidList = listNew(DYNAMIC);
    pgidList = listNew(DYNAMIC);
    comandos = listNew(DYNAMIC);


    while(1){
        printf("gsh>");
        line = utilsGetLine();
        if(ignoreRead){
            ignoreRead = 0;
            free(line);
            continue;
        }
        fflush(NULL);
        printf("%s\n" ,line);
        clearInput(line, comandos);
        free(line);

        int i;
        listRestart(comandos);
        initialCommandSize = listGetLength(comandos);
        for(i = 0; i < initialCommandSize; i++){
            char* comando = listRemove(comandos, 0);
            int selectedCommand = isGhostCommand(comando);
            if(selectedCommand == 1){
                //myWait
                myWait();

            } else if(selectedCommand == 2){
                //cleanANDdie
                cleanAndDie();
            } else{
                //normalCommand
                pid = fork();
                if(i == 0){
                    pgid = pid;
                }
                if(pid == -1){
                    perror("NAO CONSEGUI FAZER UM FILHO");
                } else if(pid == 0){ // son
                    generateGhostSon();
                    signal(2, SIG_IGN);
                    signal(20, oldSigStopHandler);
                    signal(17, oldSigChildHandler);
                    if(initialCommandSize != 1){
                        setpgid(getpid(), pgid);
                    }
                    char* matrix[strlen(comando)];
                    createMatrix(comando, matrix);
                    printf("comando: %s\n", comando);
                    execvp(matrix[0], matrix);
                    perror("NAO CONSEGUI EXECUTAR ESSE COMANDO");
                    exit(0);
                    printf("DEI EXIT");
                }else{ // father
                    pid_t* pidPointer = (pid_t*) malloc(sizeof(pid_t));
                    *pidPointer = pid;
                    listAdd(pidList, pidPointer);

                    if(initialCommandSize != 1){
                        pid_t* pgidPointer = (pid_t*) malloc(sizeof(pid_t));
                        *pgidPointer = pgid;
                        listAdd(pgidList, pgidPointer);
                    }
                    if(initialCommandSize == 1){
                        waitpid(pid, NULL, NULL);
                    }
                }

            }
        }
        clean(comandos);
    }
}

