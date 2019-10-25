//
// Created by otavio on 24/10/2019.
//
#include "utils.h"

extern List pidList;
extern List pgidList;
extern List comandos;

struct string{
    char* array;
    unsigned int size;
    unsigned int capacity;
};

char* utilsGetLine(){
    char aux[51] = {'\0'};
    char* str;
    str = (char*)malloc(sizeof(char));
    *str = '\0';
    int sizeAux = 0;
    int sizeStr = 0;
    int totalSize = 0;
    while (fgets(aux, 50, stdin) != EOF){
        sizeAux = strlen(aux);
        sizeStr = strlen(str);
        totalSize = sizeAux + sizeStr + 1;
        str = (char*)realloc(str, totalSize* sizeof(char));

        strcat(str, aux);
        if(str[totalSize-2] == '\n'){
            str[totalSize-2] = '\0';
            break;
        }
    }
    return str;
}

void clearInput(char *linha, List this){

    int i = -1;
    char *tokenizer = strtok(linha, "#");
    int size = 0;
    while (tokenizer != NULL){
        i++;
        size = (int) (strlen(tokenizer) + 1);
        char* aux = (char*) malloc(size * sizeof(char));
        strcpy(aux,tokenizer);
        listAdd(this, aux);
        tokenizer = strtok(NULL, "#");
    }
}

int isGhostCommand(char* command){
    if (strcmp(command, "mywait") == 0) {
        return 1;
    }
    if (strcmp(command, "clean&die") == 0) {
        return 2;
    }
    return 0;
}

void generateGhostSon(){
    srand(time(NULL));
    if(rand() % 2){
        printf("GHOST\n");
        fork();
    }
}

char** createMatrix(char* command, char** matrix){
    int i = -1;
    char* tokenizer = strtok(command, " ");
    while(tokenizer != NULL){
        i++;
        matrix[i] = tokenizer;
        tokenizer = strtok(NULL, " ");
    }
    matrix[i+1] = NULL;
}

int myWait(){
    int info = 1;
    while(info > 0){
        info = waitpid(-1, NULL, WNOHANG);
    }
}

int cleanAndDie(){

        sigset_t mask;
        sigfillset(&mask);
        sigprocmask(SIG_SETMASK, &mask, NULL);

        int info = 1;
        int tam = listGetLength(pgidList);
        int i;
        listRestart(pgidList);
        for(i = 0; i < tam; i++){
            pid_t* pgid = (pid_t*)listNext(pgidList);
            kill(-(*pgid), SIGKILL);
        }
        while(info > 0){
            info = waitpid(-1, NULL, WNOHANG);
        }
        listFree(pgidList);
        listFree(pidList);
        listFree(comandos);
        exit(0);
}

void sigIntHandler(int sig){
    int childRunning = 0;
        pid_t* pidPointer;
        int i;
        listRestart(pidList);
        int initialSize = listGetLength(pidList);
        for(i=0; i < initialSize; i++){
            pidPointer = (pid_t*)listNext(pidList);
            int status;
            printf("oid\n");
            pid_t return_pid = waitpid(*pidPointer, &status, WNOHANG); /* WNOHANG def'd in wait.h */
            if (return_pid == -1) {
                /* error */
                perror("ERROR IN SIGINT HANDLER\n");
            } else if (return_pid == 0) {
                /* child is still running */
                childRunning = 1;
            } else if (return_pid == *pidPointer) {
                /* child is finished. exit status in   status */
           }
        }

        if(childRunning){
            char option[10];
            printf("EXISTEM PROCESSOS ATIVOS. DESEJA FINALIZAR A SHELL? S/N\n");
            scanf("%s", option);
            if(strcmp(option, "S") == 0 || strcmp(option, "s") == 0 ){
                exit(0);

            }
        }else{
            exit(0);
        }

}

