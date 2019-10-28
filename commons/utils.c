//
// Created by otavio on 24/10/2019.
//
#include "utils.h"

extern List pidList;
extern List pgidList;
extern List comandos;
extern int ignoreRead;
char* gambiarraLine;

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


    while (!ignoreRead && fgets(aux, 50, stdin) != NULL){
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
	if(ignoreRead){
		ignoreRead = 0;
		return gambiarraLine;
	}else{
    		return str;
	}

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
            pid_t return_pid = waitpid(-1, &status, WNOHANG); /* WNOHANG def'd in wait.h */
            if (return_pid == -1) {
                /* error */
                childRunning = 0;

            } else if (return_pid == 0) {
                /* child is still running */
                childRunning = 1;
            } else if (return_pid == *pidPointer) {
                /* child is finished. exit status in   status */
           }
        }

        if(childRunning){
            char option;
            printf("EXISTEM PROCESSOS ATIVOS. DESEJA FINALIZAR A SHELL? S/N\n");
            scanf("%c", &option);
		clearBuffer();
            if(option == 's' || option == 'S'){
                exit(0);
            }else{
                printf("gsh>");
		ignoreRead = 1;
		gambiarraLine = utilsGetLine();
            }
        }else{
            exit(0);
        }
}

void sigChildHandler(int sig) {
    int childRunning = 0;
    int status;
    pid_t* pidPointer;
    int i;
    listRestart(pidList);
    int initialSize = listGetLength(pidList);
    for(i=0; i < initialSize; i++){
        printf("blau\n");
        pidPointer = (pid_t*)listNext(pidList);
        if(pidPointer == NULL){
            continue;
        }
        pid_t return_pid = waitpid(*pidPointer, &status, WNOHANG); /* WNOHANG def'd in wait.h */
        if (return_pid == -1) {
            /* error */
        } else if (return_pid == 0) {
            /* child is still running */
            childRunning = 1;
        } else if (return_pid == *pidPointer) {
            if(!WIFEXITED(status)){
                pid_t* pidPointer = NULL;
                pid_t* pgidPointer = NULL;
                pid_t pgid;
                pidPointer = listRemoveByPid(pidList, return_pid);
                pgid = getpgid(*pidPointer);
                printf("PGID: %d\n", pgid);
                if(pidPointer != NULL){
                    pgidPointer = listRemoveByPid(pgidList, pgid);
                    if(pgidPointer !=  NULL){
                        kill(-pgid, SIGKILL);
                        free(pgidPointer);
                    }
                    free(pidPointer);
                }
            }else{
                free(listRemoveByPid(pidList, return_pid));
            }
            /* child is finished. exit status in   status */

        }
    }
}

void sigStopHandler(int sig) {


    int i, initialSize;
    pid_t* pgidPointer;
    initialSize = listGetLength(pgidList);
    listRestart(pgidList);
    for (i=0;  i  < initialSize; i++) {
        pgidPointer = listNext(pgidList);

        kill(-(*pgidPointer), SIGTSTP);
    }
}

void clearBuffer(){
	int c;
	while ((c = getchar()) != '\n' && c != EOF) { }
}






























