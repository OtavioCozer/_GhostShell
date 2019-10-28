#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "commons/List.h"
#include "commons/utils.h"


//estrutura de dados globais para serem utilizadas nos handlers
List pidList;
List pgidList;
List comandos;
int ignoreRead;

int main() {
    ignoreRead = 0;
    void (*oldSigChildHandler)(int);
    void (*oldSigStopHandler)(int);
    signal(SIGINT, sigIntHandler);
    oldSigChildHandler = signal(SIGCHLD, sigChildHandler);
    oldSigStopHandler = signal(SIGTSTP, sigStopHandler);

    int initialCommandSize = 0;
    char *line;
    pid_t pgid = 0;
    pid_t pid = 0;
    pidList = listNew(DYNAMIC);
    pgidList = listNew(DYNAMIC);
    comandos = listNew(DYNAMIC);

    //loop responsavel por manter a shell executando
    while (1) {
        printf("gsh>");
        line = utilsGetLine();
        if (ignoreRead) {
            ignoreRead = 0;
            continue;
        }
        clearInput(line, comandos);
        free(line);

        int i;
        listRestart(comandos);
        initialCommandSize = listGetLength(comandos);
        //loop que trata os comandos separados por #
        for (i = 0; i < initialCommandSize; i++) {
            char *comando = listRemove(comandos, 0);
            int selectedCommand = isGhostCommand(comando);
            if (selectedCommand == 1) {
                //myWait
                myWait();
                initialCommandSize--;
            } else if (selectedCommand == 2) {
                //cleanANDdie
                cleanAndDie();
                initialCommandSize--;
            } else {
                //normalCommand
                pid = fork();
                if (i == 0) {
                    pgid = pid;
                }
                if (pid == -1) {
                    perror("NAO CONSEGUI FAZER UM FILHO");
                } else if (pid == 0) { // son
                    generateGhostSon();
                    signal(SIGINT, SIG_IGN);
                    signal(SIGTSTP, oldSigStopHandler);
                    signal(SIGCHLD, oldSigChildHandler);
                    if (initialCommandSize != 1) {
                        setpgid(getpid(), pgid);
                    }
                    char *matrix[strlen(comando)];
                    createMatrix(comando, matrix);
                    execvp(matrix[0], matrix);
                    perror("NAO CONSEGUI EXECUTAR ESSE COMANDO");
                    exit(0);
                    printf("DEI EXIT");
                } else { // father
                    //inseri o pid do filho na lista de filhos
                    pid_t *pidPointer = (pid_t *) malloc(sizeof(pid_t));
                    *pidPointer = pid;
                    listAdd(pidList, pidPointer);

                    //inserir o pgid do grupo na lista de grupos
                    if (initialCommandSize != 1 && i == 0) {
                        pid_t *pgidPointer = (pid_t *) malloc(sizeof(pid_t));
                        *pgidPointer = pgid;
                        listAdd(pgidList, pgidPointer);
                    }
                    if (initialCommandSize == 1) {
                        waitpid(pid, NULL, 0);
                    }
                }

            }
        }
        clean(comandos);
    }
}

