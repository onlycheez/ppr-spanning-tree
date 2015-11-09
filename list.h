
typedef struct point {
    void *data;
    struct point *next;
} node;

void *list_pop(node ** first);
node* list_push(node *first, void *value);
node *list_copy(node * first);
void list_enque(node ** first, void *value);
int list_size(node * first);
void list_free(node *root);

