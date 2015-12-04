
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"

void find_solution(list_node** solutions, list_node* current_solution, int size)
{
  int current_solution_node_idx = list_size(current_solution) - size;
  list_node *current_solution_node = list_at_index(current_solution,
    current_solution_node_idx);
  graph_node *current_node = current_solution_node->data;

  int i;
  for (i = current_node->neighbors_count - 1; i >= 0 ; i--)
  {
    graph_node *neighbor = current_node->neighbors[i];

    if ((neighbor->visited))
    {
      continue;
    }

    list_node *new_solution = list_copy(current_solution);
    list_enque(&new_solution, neighbor);

    if (list_size(new_solution) == (2 * size))
    {
      *solutions = list_push(*solutions, new_solution);
    }
    else
    {
      if (neighbor->id != DUMMY_NODE_ID)
      {
        neighbor->visited = 1;
      }

      find_solution(solutions, new_solution, size);
      neighbor->visited = 0;
      list_free(new_solution);
    }
  }
}

void dump_solution(list_node *root)
{
  list_node *tmp = root;
  printf("%d\n", ((graph_node *) tmp->data)->id);
  tmp = tmp->next;

  int i, line_length = 1;

  while (tmp)
  {
    for (i = 0; i < line_length; i++)
    {
      printf("%02d ", ((graph_node *) tmp->data)->id);
      tmp = tmp->next;
    }
    printf("\n");
    line_length *= 2;
  }
}

void mark_nodes(list_node *root, int is_visited)
{
  list_node *tmp = root;

  while (tmp)
  {
    graph_node *node = (graph_node *) tmp->data;

    if (node->id != DUMMY_NODE_ID)
    {
      node->visited = 1;
    }

    tmp = tmp->next;
  }
}

void mark_nodes_visited(list_node *root)
{
  mark_nodes(root, 1);
}

void mark_nodes_unvisited(list_node *root)
{
  mark_nodes(root, 0);
}

double best_height = 1000.0;
list_node *best_solution;

int main(int argc, char *argv[])
{
  struct graph *graph = graph_new_from_file(argv[1]);
  int ideal_height = ceilf(log2(graph->size)) + 1;

  list_node *solution = list_push(NULL, graph->root);
  list_node *solutions = list_push(NULL, solution);

  while (list_size(solutions) > 0)
  {
    list_node *current_solution = list_pop(&solutions);
    mark_nodes_visited(current_solution);
    double height = log2(list_size(current_solution)) + 1;

    if (graph_all_visited(graph))
    {
      if (height < best_height)
      {
        best_height = height;
        best_solution = current_solution;
        if (height == ideal_height)
        {
          break;
        }
      }
    }
    else if (height < best_height)
    {
      find_solution(&solutions, current_solution, list_size(current_solution));
    }

    mark_nodes_unvisited(current_solution);
    list_free(current_solution);
  }

  dump_solution(best_solution);
  graph_free(graph);

  return 0;
}
