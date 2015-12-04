
typedef struct _list_node {
    void *data;
    struct _list_node *next;
} list_node;

void *list_pop(list_node ** first);
list_node* list_push(list_node *first, void *value);
list_node *list_copy(list_node * first);
void list_enque(list_node ** first, void *value);
int list_size(list_node * first);
void list_free(list_node *root);
list_node* list_at_index(list_node *root, int index);
