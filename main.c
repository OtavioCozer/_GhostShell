#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "commons/List.h"
#include "commons/utils.h"

List pidList;
List pgidList;
List comandos;

int main() {

    char* line;
    pid_t pgid = 0;
    pidList = listNew(STATIC);
    pgidList = listNew(STATIC);
    comandos = listNew(DYNAMIC);


    while(1){
        printf("gsh>");
        line = utilsGetLine();
        printf("TO PRINTANDO LINE: %s\n", line);
        free(line);
    }
}