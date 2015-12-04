
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

static graph_node* graph_new_node(char id)
{
  graph_node* node = malloc(sizeof(graph_node));
  node->id = id;
  node->visited = 0;
  node->neighbors_count = 0;
  node->neighbors = NULL;

  return node;
}

static graph_node** graph_init_node_array(int count)
{
  graph_node** array = malloc(count * sizeof(graph_node *));

  int i;
  for (i = 0; i < count; i++)
  {
    array[i] = graph_new_node(i);
  }

  return array;
}

static void graph_set_node_neighbors(graph_node *node, const char *line,
  graph_node **nodes, int count)
{
  graph_node **neighbors = malloc(count * sizeof(graph_node *));

  int i;
  for (i = 0; i < count; i++)
  {
    if (line[i] == '1')
    {
      neighbors[node->neighbors_count] = nodes[i];
      node->neighbors_count += 1;
    }
  }

  // Add last dummy neighbor
  neighbors[node->neighbors_count] = nodes[count];
  node->neighbors_count += 1;

  node->neighbors = malloc(node->neighbors_count * sizeof(graph_node *));
  memcpy(node->neighbors, neighbors, (node->neighbors_count) * sizeof(graph_node *));
  free(neighbors);
}

static graph_node* graph_init_dummy_node()
{
  graph_node *dummy_node = graph_new_node(DUMMY_NODE_ID);
  dummy_node->neighbors = malloc(sizeof(graph_node *));
  dummy_node->neighbors[0] = dummy_node;
  dummy_node->neighbors_count = 1;

  return dummy_node;
}

struct graph* graph_init_empty(void)
{
  struct graph* graph = malloc(sizeof(struct graph));
  graph->root = NULL;
  graph->size = 0;

  return graph;
}

struct graph* graph_new_from_file(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    perror("graph_new_from_file");
    exit(1);
  }

  struct graph* graph = graph_init_empty();
  graph->size = graph_read_nodes_count(file);

  int line_length = graph->size + 1; // +1 for new line character.
  printf("Nodes count: %d\n", graph->size);
  // + 1 for dummy vertex
  graph->nodes = graph_init_node_array(graph->size + 1);
  int node_nr = 0;
  char *line = malloc(line_length);

  graph->nodes[graph->size] = graph_init_dummy_node();

  while (node_nr < graph->size)
  {
    if (getline(&line, &line_length, file) < 0)
    {
      perror("graph_new_from_file");
      exit(1);
    }

    graph_set_node_neighbors(graph->nodes[node_nr], line,
      graph->nodes, graph->size);
    node_nr += 1;
  }

  free(line);

  graph->root = graph->nodes[0];

  return graph;
}

static void graph_dump_level(graph_node *node, int indent)
{
  if (node->visited)
  {
    return;
  }

  node->visited = 1;

  int j;
  for (j = 0; j < indent; j++)
  {
    printf(" ");
  }
  printf("%d (%d)\n", node->id, node->neighbors_count);


  int i;
  for (i = 0; i < node->neighbors_count; ++i)
  {
    graph_dump_level(node->neighbors[i], indent + 2);
  }
}

void graph_dump(graph_node *root)
{
  graph_dump_level(root, 0);
}

int graph_all_visited(struct graph *graph)
{
  int i = 0;

  while (graph->nodes[i]->id != DUMMY_NODE_ID)
  {
    if (graph->nodes[i]->visited == 0)
    {
      return 0;
    }
    i += 1;
  }

  return 1;
}

static void graph_free_vertex(graph_node *vertex)
{
  free(vertex->neighbors);
  free(vertex);
}

void graph_free(struct graph *graph)
{
  int i;

  for (i = 0; i < graph->size; i++)
  {
    graph_free_vertex(graph->nodes[i]);
  }

  // Delete the extra dummy node.
  graph_free_vertex(graph->nodes[graph->size]);

  free(graph->nodes);
  free(graph);
}
