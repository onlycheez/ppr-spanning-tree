
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"

node* list_at_index(node *root, int index)
{
  if (!root)
  {
    return NULL;
  }

  node *tmp = root;
  int i = 0;
  while (i < index)
  {
    tmp = tmp->next;
    i += 1;
  }

  return tmp;
}

void find_path(node** stack_top, node* list, int size)
{
  int node_idx = list_size(list) - size;

  node *current_node = list_at_index(list, node_idx);
  struct vertex *current_vertex = current_node->data;

  int i;
  for (i = current_vertex->neighbors_count - 1; i >= 0 ; i--)
  {
    struct vertex *neighbor = current_vertex->neighbors[i];

    if (!(neighbor->visited))
    {
      node *top = list_copy(list);
      list_enque(&top, neighbor);

      if (list_size(top) >= (2 * size))
      {
        *stack_top = list_push(*stack_top, (void *)top);
      }
      else
      {
        if (neighbor->id != DUMMY_NODE_ID)
        {
          neighbor->visited = 1;
        }

        find_path(stack_top, top, size);
        neighbor->visited = 0;
        list_free(top);
      }
    }
  }
}

void dump_solution(node *list)
{
  node *tmp = list;
  printf("%d\n", ((struct vertex *) tmp->data)->id);
  tmp = tmp->next;

  int i, line_length = 1;

  while (tmp)
  {
    for (i = 0; i < line_length; i++)
    {
      printf("%02d ", ((struct vertex *) tmp->data)->id);
      tmp = tmp->next;
    }
    printf("\n");
    line_length *= 2;
  }
}

double best_height = 1000.0;
node *best_solution;

int main(int argc, char *argv[])
{
  struct graph *graph = graph_new_from_file(argv[1]);
  node *record = list_push(NULL, graph->nodes_array[0]);
  node *top = list_push(NULL, record);

  int ideal_height = ceilf(log2(graph->size)) + 1;

  while (list_size(top) > 0)
  {
    node *current_record = list_pop(&top);
    node *record_tmp = current_record;

    printf("Record: ");
    while (record_tmp)
    {
      struct vertex *current_vertex = (record_tmp->data);
      printf("%d, ", current_vertex->id);
      if (current_vertex->id != DUMMY_NODE_ID)
      {
        current_vertex->visited = 1;
      }

      record_tmp = record_tmp->next;
    }
    printf("\n");

    double height = log2(list_size(current_record)) + 1;

    if (graph_all_visited(graph))
    {
      if (height < best_height)
      {
        best_height = height;
        best_solution = current_record;
        if (height == ideal_height)
        {
          goto end;
        }
      }
    }
    else if (height < best_height)
    {
      find_path(&top, current_record, list_size(current_record));
    }

    record_tmp = current_record;
    while (record_tmp)
    {
      struct vertex *current_vertex = (record_tmp->data);
      current_vertex->visited = 0;
      record_tmp = record_tmp->next;
    }

    list_free(current_record);
  }

end:
  dump_solution(best_solution);

  return 0;
}
