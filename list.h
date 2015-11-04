
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
struct node* list_new_from_file(const char *filename);

void list_print(struct node *root);
