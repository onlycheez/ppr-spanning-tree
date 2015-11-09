
#define DUMMY_NODE_ID -1

struct vertex
{
  int id;
  char visited;
  struct vertex **neighbors;
  int neighbors_count;
};

/**
 * Reads graph represented as adjacency matrix from file.
 */
struct vertex** graph_new_from_file(const char *filename, int *nodes_count);

void graph_dump(struct vertex *root);
