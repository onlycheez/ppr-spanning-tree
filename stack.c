/*
 * File:   main.cpp
 * Author: Hummej
 *
 * Created on 25. říjen 2015, 22:32
 */

#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

void * stack_pop(node ** first){
    node * pom=NULL;
    void * retval;
    if(*first==NULL){
        return NULL;
    }
    else {
    pom=(*first)->next;
    retval= (*first)->v;
    *first=pom;
    return retval;}
};
void stack_push(node ** first,void * value){
    node *pom=NULL;
    pom=(node*)(malloc(sizeof(node)));
    pom->next=*first;
    pom->v=value;
    *first=pom;

};

node * stack_copy(node * first){
    node *first2=NULL;
    node *pom=NULL;
    node *pom2=NULL;
    first2 = malloc(sizeof(node));
    pom=first2;
    if (first){
        while (first->next){
            pom->v=first->v;
            pom2=pom;
            pom= malloc(sizeof(node));
            pom2->next=pom;
            first=first->next;
        }
        pom->v=first->v;
        pom->next=NULL;
    }
    return first2;
};

void stack_enque(node ** first,void * value){
    node *pom=NULL;
    pom=malloc(sizeof(node));
    node *pom2=NULL;
    pom->next=NULL;
    pom->v=value;
    pom2=*first;

    if (!pom2){
        *first=pom;
    }
    else{
    while (pom2->next) {
        pom2=pom2->next;
    }
    pom2->next=pom;
    }
};
int stack_size(node * first){
    node *pom=NULL;
    int retval=0;
    int i=0;
    pom=malloc(sizeof(node));
    pom=first;
    while (pom) {
        retval++;
        pom=pom->next;


    }
    return retval;

}
