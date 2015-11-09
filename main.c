
#include "graph.h"
#include "list.h"

int main(int argc, char *argv[])
{
  struct vertex **graph = graph_new_from_file(argv[1]);

  node *record = malloc(sizeof(node));
  list_push(&record, graph[0]);

  node* top = malloc(sizeof(node));
  list_push(&top, record);

  while (list_size(top) > 0)
  {
    node *current_record = list_pop(&top);
    //vertex *vertex_tmp = record;

    while (current_record)
    {
      vertex current_vertex = (vertex *)(current_record->v);
      current_vertex->visited = 1;

      current_record = current_record->next;
    }
  }

  return 0;
}
