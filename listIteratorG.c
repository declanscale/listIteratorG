// listIteratorG.c ... Generic List Iterator implementation
// Written by ....

/*
    You need to submit ONLY this file....

*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"
#include <ctype.h>

typedef struct Node * node;

struct Node {

    void *value;
    node prev;
    node next;


};

typedef struct IteratorGRep {

    node prevNode;
    node nextNode;
    node lastReturn;
    node head;
    node tempNext;
    node tempPrev;

    ElmCompareFp cmpFp;
    ElmCopyFp copyFp;
    ElmFreeFp freeFp;

    //should add a isvalid value element

} IteratorGRep;


IteratorG IteratorGNew(ElmCompareFp cmp, ElmCopyFp copy, ElmFreeFp free){

    IteratorGRep *newIterator = malloc(sizeof(IteratorGRep));


    newIterator->prevNode = NULL;
    newIterator->nextNode = NULL;
    newIterator->lastReturn = NULL;
    newIterator->head = NULL;
    newIterator->tempNext = NULL;
    newIterator->tempPrev = NULL;

    newIterator->cmpFp = cmp;
    newIterator->copyFp = copy;
    newIterator->freeFp = free;


    return newIterator;

}


//----------------------------------------------------------


int  add(IteratorG it, void *vp){


    node newNode = malloc(sizeof(struct Node));

    int ifadd = 0;

    if(it->head == NULL){ //there is no elements in the list

        it->prevNode = newNode;
        it->nextNode = NULL;
        it->head = newNode;
        it->prevNode->value = it->copyFp(vp);
        ifadd = 1;

    }else{

       if(it->nextNode == NULL){ //cursor at the end of the list

            it->prevNode->next = newNode;
            it->prevNode->next->prev = it->prevNode;
            it->prevNode = it->prevNode->next;
            it->nextNode = NULL;
            it->prevNode->value = it->copyFp(vp);
            ifadd = 1;

       }else if(it->prevNode == NULL){ //cursor at the front of the list

            it->nextNode->prev = newNode;
            it->nextNode->prev->next = it->nextNode;
            it->prevNode = it->nextNode->prev;
            it->head = it->prevNode;
            it->prevNode->value = it->copyFp(vp);
            ifadd = 1;

       }else{   // cursor at the middle of the list

            it->nextNode->prev = newNode;
            it->nextNode->prev->next = it->nextNode;
            it->nextNode->prev->prev = it->prevNode;
            it->prevNode->next = it->nextNode->prev;
            it->prevNode = it->nextNode->prev;
            it->prevNode->value = it->copyFp(vp);
            ifadd = 1;
       }

    }

  return ifadd;

}


int  hasNext(IteratorG it){

    if(it->nextNode != NULL){
        return 1;
    }else{
        return 0;
    }

}

int  hasPrevious(IteratorG it){

    if(it->prevNode != NULL){
        return 1;
    }else{
        return 0;
    }
    
}

void *next(IteratorG it){

    void *pNext = NULL;

    if(it->nextNode == NULL){
        return pNext;
    }else{

        if(it->nextNode->next == NULL){

            pNext = it->nextNode->value;
            it->lastReturn = it->nextNode;
            it->prevNode = it->nextNode;
            it->nextNode = NULL;
            return pNext;

        }else{

            pNext = it->nextNode->value;
            it->lastReturn = it->nextNode;
            it->prevNode = it->nextNode;
            it->nextNode = it->nextNode->next;
            return pNext;

        }
    }


}

void *previous(IteratorG it){

    void *pPrev = NULL;

    if(it->prevNode == NULL){

        return pPrev;

    }else{

        if(it->prevNode->prev == NULL){

            pPrev = it->prevNode->value;
            it->lastReturn = it->prevNode;
            it->nextNode = it->prevNode;
            it->prevNode = NULL;
            return pPrev;

        }else{

            pPrev = it->prevNode->value;
            it->lastReturn = it->prevNode;
            it->nextNode = it->prevNode;
            it->prevNode = it->prevNode->prev;
            return pPrev;
        }
    }

}


void *findNext(IteratorG it, void *vp){

    void *pFindNext = NULL;
    int ifFind = 0;
    it->tempNext = it->nextNode;
    it->tempPrev = it->prevNode;

    if(it->nextNode == NULL){
        pFindNext = NULL;
    }else{
        while(ifFind == 0 && it->nextNode != NULL){

                if(it->cmpFp(it->nextNode->value, vp) == 0){
                    pFindNext = &it->prevNode->value;
                    it->lastReturn = it->prevNode;
                    ifFind = 1;
                }else{
                    it->prevNode = it->nextNode;
                    it->nextNode = it->nextNode->next;
                }

        }

        if(ifFind == 0){
            it->nextNode = it->tempNext;
            it->prevNode = it->tempPrev;
            pFindNext = NULL;
        }
    }

  return pFindNext;

}


void *findPrevious(IteratorG it, void *vp){

    void *pFindPrev = NULL;
    int ifFind = 0;
    it->tempNext = it->nextNode;
    it->tempPrev = it->prevNode;

    if(it->prevNode == NULL){
        pFindPrev = NULL;
    }else{
        while(ifFind == 0 && it->prevNode != NULL){

                if(it->cmpFp(it->prevNode->value, vp) == 0){
                    pFindPrev = &it->nextNode->value;
                    it->lastReturn = it->nextNode;
                    ifFind = 1;

                }else{
                    it->nextNode = it->prevNode;
                    it->prevNode = it->prevNode->prev;
                }

        }
        if(ifFind == 0){
            it->tempNext = it->nextNode;
            it->tempPrev = it->prevNode;
            pFindPrev = 0;
        }
    }

  return pFindPrev;

}


int  set(IteratorG it, void *vp){

    int ifset = 0;

    if(it->lastReturn == NULL){
        return ifset;
    }else{

        it->lastReturn->value = it->copyFp(vp);
        ifset = 1;
    }


    return ifset;


}


int  delete(IteratorG it){

    node temp = it->lastReturn;
    int ifDelete;

    if(it->lastReturn == NULL){

        ifDelete = 0;

    }else{

        if(it->lastReturn->prev == NULL){  //lastReturn is the first node of the list

            if(it->prevNode == NULL){//prevNode is NULL

                it->nextNode = it->nextNode->next;
                it->nextNode->prev = NULL;
                it->head = it->nextNode;
                it->freeFp(temp);
                ifDelete = 1;
            }else{
                it->prevNode = NULL;
                it->nextNode->prev = NULL;
                it->head = it->nextNode;
                it->freeFp(temp);
                ifDelete = 1;
            }

        }else if(it->lastReturn->next == NULL){//lastReturn is the last node in the list

            if(it->nextNode == NULL){
                it->prevNode = it->prevNode->prev;
                it->prevNode->next = NULL;
                it->freeFp(temp);
                ifDelete = 1;
            }else{
                it->nextNode = NULL;
                it->prevNode->next = NULL;
                it->freeFp(temp);
                ifDelete = 1;
            }
        }else{ //lastReturn at the middle of the list

            if(it->nextNode == it->lastReturn){ //lastReturn by findprev or previous
                it->prevNode->next = it->nextNode->next;
                it->nextNode->next->prev = it->prevNode;
                it->nextNode = it->nextNode->next;
                it->freeFp(temp);
                ifDelete = 1;

            }else if(it->prevNode == it->lastReturn){ //lastReturn by findnext or next
                it->nextNode->prev = it->prevNode->prev;
                it->prevNode->prev->next = it->nextNode;
                it->prevNode = it->prevNode->prev;
                it->freeFp(temp);
                ifDelete = 1;

            }
        }
    }

    return ifDelete;


}


void reset(IteratorG it){

     if(it->prevNode == NULL){

    }else{
        it->nextNode = it->head;
        it->prevNode = NULL;
    }

}





// -------------------------------------- the end






