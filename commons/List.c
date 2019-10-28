//
// Created by otavio on 22/10/2019.
//

#include "List.h"

typedef struct cell* Cell;

struct cell{
    Cell next;
    void* item;
};

typedef struct list_{
    Cell start;
    Cell end;
    Cell current;
    unsigned int isDynamic;
    unsigned int length;
    unsigned int begin;
    unsigned int currentRemoved;
}list;

List listNew(unsigned int isDynamic){
    List aux = (List)malloc(sizeof(list));
    aux->length = 0;
    aux->begin = 0;
    aux->currentRemoved = 0;
    aux->start = NULL;
    aux->end = NULL;
    aux->current = NULL;
    aux->isDynamic = isDynamic;
    return aux;
}

Cell cellNew(){
    Cell aux = (Cell)malloc(sizeof(struct cell));
    aux->item = NULL;
    aux->next = NULL;
}

void* listGet(List this, unsigned int index){
    int i;
    this->current = this->start;
    for(i = 0; i < index && this->current!= NULL; i++){
        this->current = this->current->next;
    }
    if(this->current == NULL){
        perror("LIST GET ERROR: INDEX OUT OF LIST BOUNDS\n");
        return NULL;
    }
    return this->current->item;
}

void listFree(List this){
    Cell current = this->start;
    Cell previous = this->start;
    while(current != NULL){
        void* item = current->item;

        previous = current;
        current = current->next;
        free(previous);
        if(this->isDynamic){
            free(item);
        }
    }
    free(this);
}

void listAdd(List this, void* item){
    Cell newCell = cellNew();
    newCell->item = item;

    if(this->length == 0 && this->start == NULL && this->end == NULL){
        this->start = newCell;
        this->end = newCell;
        this->length++;
    } else {
        this->end->next = newCell;
        this->end = newCell;
        this->length++;
    }
}

void* listRemove(List this, unsigned int index){
    if(this->start != NULL && this->end != NULL ) {
        int i;
        Cell current = this->start;
        Cell previous = this->start;
        for (i = 0; i < index && current != NULL; i++) {
            previous = current;
            current = current->next;
        }
        if (current == NULL) {
            perror("LIST REMOVE ERROR: INDEX OUT OF LIST BOUNDS\n");
            return NULL;
        }
        if (this->start == this->end) {
            void *item = current->item;
            free(current);
            this->start = NULL;
            this->end = NULL;
            this->length--;
            return item;
        }
        if(this->start == current){
            this->start = current->next;
            void* item = current->item;
            free(current);
            this->length--;
            return item;
        }
        if(this->end == current){
            this->end = previous;
            previous->next = NULL;
            void* item = current->item;
            free(current);
            this->length--;
            return item;
        }
        previous->next = current->next;
        void* item = current->item;
        free(current);
        this->length--;
        return item;
    }else{
        perror("LIST REMOVE ERROR: EMPTY LIST");
        return NULL;
    }
}

int listGetLength(List this) {
    return this->length;
}

void listRestart(List this){
    this->begin = 1;
}

void* listNext(List this){
    if(this != NULL) {
        if(this->currentRemoved){
            this->currentRemoved = 0;
            return this->current;
        }
        if (this->begin == 1) {
            this->current = this->start;
            this->begin = 0;
            return this->current->item;
        } else {
            if(this->current == NULL){
                return NULL;
            }
            this->current = this->current->next;
            return this->current->item;
        }
    }else{
        perror("LIST NEXT: NULL POINTER\n");
        return NULL;
    }
}

char** list2Matrix(List this){
    if(this->start!=NULL && this->end!=NULL){
        char* matrix[this->length+1];
        listRestart(this);
        int i;
        for(i = 0; i < this->length; i++){
            matrix[i] = listNext(this);
        }
        matrix[this->length] = NULL;
        return matrix;
    } else{
        perror("LIST TO MATRIX: NULL POINTER\n");
        return NULL;
    }
}

void clean(List this){
    Cell current = this->start;
    Cell previous = this->start;
    while(current != NULL){
        void* item = current->item;

        previous = current;
        current = current->next;
        free(previous);
        if(this->isDynamic){
            free(item);
        }
    }
    free(this);
    this = listNew(DYNAMIC);
}

void* listRemoveByPid(List this, pid_t pid){
    if(this->start != NULL && this->end != NULL ) {
        int i;
        Cell current = this->start;
        Cell previous = this->start;
        pid_t* pidPointer;
        for (i = 0; current != NULL; i++) {
            pidPointer = (pid_t*)current->item;
            if(pid == *pidPointer){
                break;
            }
            previous = current;
            current = current->next;
        }

        if (current == NULL) {
            perror("LIST REMOVE ERROR: INDEX OUT OF LIST BOUNDS\n");
            return NULL;
        }
        if(current == this->current){
            this->current = this->current->next;
            this->currentRemoved = 1;
        }
        if (this->start == this->end) {
            void *item = current->item;
            free(current);
            this->start = NULL;
            this->end = NULL;
            this->length--;
            return item;
        }
        if(this->start == current){
            this->start = current->next;
            void* item = current->item;
            free(current);
            this->length--;
            return item;
        }
        if(this->end == current){
            this->end = previous;
            previous->next = NULL;
            void* item = current->item;
            free(current);
            this->length--;
            return item;
        }
        previous->next = current->next;
        void* item = current->item;
        free(current);
        this->length--;
        return item;
    }else{
        perror("LIST REMOVE ERROR: EMPTY LIST");
        return NULL;
    }

}



