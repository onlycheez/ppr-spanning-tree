
#include "list.h"

int main(int argc, char *argv[])
{
  struct node *root = list_new_from_file(argv[1]);
  list_print(root);

  return 0;
}
