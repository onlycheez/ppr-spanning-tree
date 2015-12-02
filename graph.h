
#define DUMMY_NODE_ID -1

struct vertex
{
  int id;
  char visited;
  struct vertex **neighbors;
  int neighbors_count;
};

struct graph
{
  int size;
  struct vertex *root;
  struct vertex **nodes_array;
};

/**
 * Reads graph represented as adjacency matrix from file.
 */
struct graph* graph_new_from_file(const char *filename);

void graph_dump(struct vertex *root);

/**
 * Free all memory associated with graph structure.
 */
void graph_free(struct graph *graph);
