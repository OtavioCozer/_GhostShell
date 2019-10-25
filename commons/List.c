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
}list;

List listNew(unsigned int isDynamic){
    List aux = (List)malloc(sizeof(list));
    aux->length = 0;
    aux->begin = 0;
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
        if (this->begin == 1) {
            this->current = this->start;
            this->begin = 0;
            return this->current->item;
        } else {
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


