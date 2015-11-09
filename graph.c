
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

static int graph_read_nodes_count(FILE *file)
{
  char *line = NULL;
  int length = 0;
  size_t r = getline(&line, &length, file);

  if (r < 0)
  {
    perror("_graph_read_nodes_count");
    exit(1);
  }

  // Delete new line character.
  line[length - 1] = '\0';

  int value = atoi(line);
  free(line);

  return value;
}

static struct vertex* graph_new_node(char id)
{
  struct vertex* node = malloc(sizeof(struct vertex));
  node->id = id;
  node->visited = 0;
  node->neighbors_count = 0;
  node->neighbors = NULL;

  return node;
}

static struct vertex** graph_init_node_array(int count)
{
  struct vertex** array = malloc(count * sizeof(struct vertex *));

  int i;
  for (i = 0; i < count; i++)
  {
    array[i] = graph_new_node(i);
  }

  return array;
}

static void graph_set_node_neighbors(struct vertex *node, const char *line,
  struct vertex **nodes_array, int count)
{
  struct vertex **neighbors = malloc(count * sizeof(struct vertex *));

  int i;
  for (i = 0; i < count; i++)
  {
    if (line[i] == '1')
    {
      neighbors[node->neighbors_count] = nodes_array[i];
      node->neighbors_count += 1;
    }
  }

  node->neighbors = malloc(node->neighbors_count * sizeof(struct vertex *));
  memcpy(node->neighbors, neighbors, (node->neighbors_count) * sizeof(struct vertex *));
  free(neighbors);
}

struct vertex** graph_new_from_file(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    perror("graph_new_from_file");
    exit(1);
  }

  int nodes_count = graph_read_nodes_count(file);
  int line_length = nodes_count + 1; // +1 for new line character.
  printf("Nodes count: %d\n", nodes_count);
  struct vertex **nodes_array = graph_init_node_array(nodes_count);
  int node_nr = 0;
  char *line = malloc(line_length);

  while (node_nr < nodes_count)
  {
    if (getline(&line, &line_length, file) < 0)
    {
      perror("graph_new_from_file");
      exit(1);
    }

    graph_set_node_neighbors(nodes_array[node_nr], line, nodes_array,
      nodes_count);
    node_nr += 1;
  }

  free(line);

  return nodes_array;
}

static void graph_dump_level(struct vertex *node, int indent)
{
  if (node->visited)
  {
    return;
  }

  node->visited = 1;

  int i;
  for (i = 0; i < indent; i++)
  {
    printf(" ");
  }
  printf("%d (%d)\n", node->id, node->neighbors_count);


  for (i = 0; i < node->neighbors_count - 1; i++)
  {
    graph_dump_level(node->neighbors[i], indent + 2);
  }
}

void graph_dump(struct vertex *root)
{
  graph_dump_level(root, 0);
}
