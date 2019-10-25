//
// Created by otavio on 24/10/2019.
//
#include "utils.h"

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
