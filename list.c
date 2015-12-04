/*
 * File:   main.cpp
 * Author: Hummej
 *
 * Created on 25. říjen 2015, 22:32
 */

#include <stdio.h>
#include <stdlib.h>
#include "list.h"

void* list_pop(list_node **first)
{
    if (!(*first))
    {
        return NULL;
    }

    list_node *tmp = (*first)->next;
    void *retval = (*first)->data;
    *first = tmp;
    return retval;
}

list_node* list_push(list_node *first, void *value)
{
    list_node *tmp = malloc(sizeof(list_node));
    tmp->next = first;
    tmp->data = value;
    return tmp;
}

list_node* list_copy(list_node *first)
{
    list_node *tmp2 = NULL;
    list_node *first2 = malloc(sizeof(list_node));
    list_node *tmp = first2;

    if (first)
    {
        while (first->next)
        {
            tmp->data = first->data;
            tmp2 = tmp;
            tmp = malloc(sizeof(list_node));
            tmp2->next = tmp;
            first = first->next;
        }
        tmp->data = first->data;
        tmp->next = NULL;
    }

    return first2;
}

void list_free(list_node *root)
{
    list_node *tmp = root;
    list_node *tmp_next;

    while (tmp)
    {
        tmp_next = tmp->next;
        free(tmp);
        tmp = tmp_next;
    }
}

void list_enque(list_node ** first,void * value)
{
    list_node *tmp = malloc(sizeof(list_node));
    tmp->next = NULL;
    tmp->data = value;

    list_node *tmp2 = *first;

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

int list_size(list_node *first)
{
    list_node *tmp = first;
    int count = 0;

    while (tmp)
    {
        count += 1;
        tmp = tmp->next;
    }

    return count;
}

list_node* list_at_index(list_node *root, int index)
{
  if (!root)
  {
    return NULL;
  }

  list_node *tmp = root;
  int i = 0;
  while (i < index)
  {
    tmp = tmp->next;
    i += 1;
  }

  return tmp;
}
