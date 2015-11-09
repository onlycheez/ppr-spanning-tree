
typedef struct point {
    void * v;
    struct point *next;
} node;

void *stack_pop(node ** first);
void stack_push(node ** first,void * value);
node *stack_copy(node * first);
void stack_enque(node ** first,void * value);
int stack_size(node * first);

