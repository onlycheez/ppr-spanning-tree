
struct node
{
  int id;
  char visited;
  struct node **neighbors;
  int neighbors_count;
};

/**
 * Reads graph represented as adjacency matrix from file.
 */
struct node* graph_new_from_file(const char *filename);

void graph_dump(struct node *root);
