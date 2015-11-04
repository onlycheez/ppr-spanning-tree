
#include "graph.h"

int main(int argc, char *argv[])
{
  struct node *root = graph_new_from_file(argv[1]);
  graph_dump(root);

  return 0;
}
