
#define DUMMY_NODE_ID -1

typedef struct _graph_node
{
  int id;
  char visited;
  struct _graph_node **neighbors;
  int neighbors_count;
} graph_node;

struct graph
{
  int size;
  graph_node *root;
  graph_node **nodes;
};

/**
 * Reads graph represented as adjacency matrix from file.
 */
struct graph* graph_new_from_file(const char *filename);

void graph_dump(graph_node *root);

/**
 * Free all memory associated with graph structure.
 */
void graph_free(struct graph *graph);
