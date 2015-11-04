
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

#define BUFFER_LENGTH 1024

static int list_read_nodes_count(FILE *file)
{
  char *line = NULL;
  int length = 0;
  size_t r = getline(&line, &length, file);

  if (r < 0)
  {
    perror("_list_read_nodes_count");
    exit(1);
  }

  // Delete new line character.
  line[length - 1] = '\0';

  int value = atoi(line);
  free(line);

  return value;
}

static struct node* list_new_node(char id)
{
  struct node* node = malloc(sizeof(struct node));
  node->id = id;
  node->visited = 0;
  node->neighbors_count = 0;
  node->neighbors = NULL;

  return node;
}

static struct node** list_init_node_array(int count)
{
  struct node** array = malloc(count * sizeof(struct node *));

  int i;
  for (i = 0; i < count; i++)
  {
    array[i] = list_new_node(i);
  }

  return array;
}

static void list_set_node_neighbors(struct node *node, const char *line,
  struct node **nodes_array, int count)
{
  struct node **neighbors = malloc(count * sizeof(struct node *));

  int i;
  for (i = 0; i < count; i++)
  {
    if (line[i] == '1')
    {
      //printf("Setting neighbor: 0x%x\n", nodes_array[i]);
      neighbors[node->neighbors_count] = nodes_array[i];
      node->neighbors_count += 1;
    }
  }

  node->neighbors = malloc(node->neighbors_count * sizeof(struct node *));
  memcpy(node->neighbors, neighbors, (node->neighbors_count) * sizeof(struct node *));
  free(neighbors);
}

struct node* list_new_from_file(const char *filename)
{
  FILE *file = fopen(filename, "r");
  if (!file)
  {
    perror("list_new_from_file");
    exit(1);
  }

  int nodes_count = list_read_nodes_count(file);
  int line_length = nodes_count + 1; // +1 for new line character.
  printf("Nodes count: %d\n", nodes_count);
  struct node **nodes_array = list_init_node_array(nodes_count);
  int node_nr = 0;
  char *line = malloc(line_length);

  while (node_nr < nodes_count)
  {
    if (getline(&line, &line_length, file) < 0)
    {
      perror("list_new_from_file");
      exit(1);
    }

    list_set_node_neighbors(nodes_array[node_nr], line, nodes_array,
      nodes_count);
    node_nr += 1;
  }

  free(line);

  struct node *root = nodes_array[0];
  free(nodes_array);

  return root;
}

static void list_print_level(struct node *node, int indent)
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
    list_print_level(node->neighbors[i], indent + 2);
  }
}

void list_print(struct node *root)
{
  list_print_level(root, 0);
}
