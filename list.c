/*
 * File:   main.cpp
 * Author: Hummej
 *
 * Created on 25. říjen 2015, 22:32
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void* list_pop(node **first)
{
    if (!(*first))
    {
        return NULL;
    }

    node *tmp = (*first)->next;
    void *retval = (*first)->data;
    *first = tmp;
    return retval;
}

void list_push(node **first, void *value)
{
    node *tmp = malloc(sizeof(node));
    tmp->next = *first;
    tmp->data = value;
    *first = tmp;
}

node* list_copy(node *first)
{
    node *tmp2 = NULL;
    node *first2 = malloc(sizeof(node));
    node *tmp = first2;

    if (first)
    {
        while (first->next)
        {
            tmp->data = first->data;
            tmp2 = tmp;
            tmp = malloc(sizeof(node));
            tmp2->next = tmp;
            first = first->next;
        }
        tmp->data = first->data;
        tmp->next = NULL;
    }

    return first2;
}

void list_enque(node ** first,void * value)
{
    node *tmp = malloc(sizeof(node));
    tmp->next = NULL;
    tmp->data = value;

    node *tmp2 = *first;

    if (!tmp2)
    {
        *first = tmp;
    }
    else
    {
        while (tmp2->next)
        {
            tmp2 = tmp2->next;
        }
        tmp2->next = tmp;
    }
}

int list_size(node * first)
{
    node *tmp = first;
    int count = 0;

    while (tmp)
    {
        count += 1;
        tmp = tmp->next;
    }

    return count;
}
