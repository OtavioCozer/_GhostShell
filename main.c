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

int main() {
    signal(SIGINT, sigIntHandler);
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

                if(pid == -1){
                    perror("NAO CONSEGUI FAZER UM FILHO");
                } else if(pid == 0){ // son
                    generateGhostSon();
                    char* matrix[strlen(comando)];
                    createMatrix(comando, matrix);
                    execvp(matrix[0], matrix);
                    perror("NAO CONSEGUI EXECUTAR ESSE COMANDO");
                    exit(0);
                    printf("DEI EXIT");
                }else{ // father
                    pid_t* pidPointer = (pid_t*) malloc(sizeof(pid_t));
                    *pidPointer = pid;
                    listAdd(pidList, pidPointer);

                    if(i == 0 && initialCommandSize != 1){
                        pgid = pid;
                        pid_t* pgidPointer = (pid_t*) malloc(sizeof(pid_t));
                        *pgidPointer = pgid;
                        listAdd(pgidList, pgidPointer);
                    }
                    if(initialCommandSize == 1){
                        waitpid(pid, NULL, NULL);
                    }
                    if(initialCommandSize != 1){
                        setpgid(pid, pgid);
                    }

                }

            }
        }
        clean(comandos);
    }
}

