
#include <math.h>
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "list.h"

#define TAG_REQUEST_DATA 1
#define TAG_DATA 2
#define TAG_ARE_YOU_FINISHED 3
#define TAG_MY_FINISH_STATUS 4
#define TAG_FINISH 5

#define MSG_LENGTH 1024 * 1024
#define MAX_IPC_LENGTH 5000

typedef struct _mpi_msg
{
  int length;
  char data[MSG_LENGTH];
} mpi_msg;

#define MSG_FINSIHED 1
#define MSG_NOT_FINSIHED 2

mpi_msg* message_serialize_state(char is_finished)
{
  mpi_msg *message = malloc(sizeof(mpi_msg));

  message->data[0] = is_finished;
  message->data[1] = 0;

  return message;
}

mpi_msg* message_serialize_data(list_node *root)
{
  mpi_msg *message = malloc(sizeof(mpi_msg));

  if (root == NULL)
  {
    message->length = 1;
    message->data[0] = -1;
    message->data[1] = 0;
  }
  else
  {

  }

  return message;
}

list_node* message_deserialize_data()
{
  return NULL;
}

int message_is_finished()
{
  return 0;
}

list_node* cut_stack(list_node *root)
{
  int size = list_size(root);

  if (size < 10)
  {
    return NULL;
  }

  int cut_idx = size / 2;

  if (cut_idx > MAX_IPC_LENGTH)
  {
    cut_idx = size - MAX_IPC_LENGTH;
  }

  list_node *tmp = list_at_index(root, cut_idx);
  list_node *new_list = tmp->next;
  tmp->next = NULL;

  return new_list;
}

int _recursion_count = 0;

void find_solution(list_node** solutions, list_node* current_solution, int size)
{
  _recursion_count += 1;
  if (_recursion_count == 50)
  {
    _recursion_count = 0;

    MPI_Status status;
    mpi_msg *message;
    char buffer[MSG_LENGTH];

    int flag = 0;
    MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

    while (flag)
    {
      MPI_Recv(&buffer, MSG_LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
        MPI_COMM_WORLD, &status);

      switch (status.MPI_TAG)
      {
        case TAG_REQUEST_DATA:
        {
          list_node *new_top = cut_stack(*solutions);
          message = message_serialize_data(new_top);
          MPI_Send(&message->data, message->length, MPI_CHAR, status.MPI_SOURCE,
            TAG_DATA, MPI_COMM_WORLD);
          free(message);
          list_free(new_top);
          break;
        }
        case TAG_DATA:
          printf("find_solution: received data from %d\n", status.MPI_SOURCE);
          break;
        case TAG_ARE_YOU_FINISHED:
          message = message_serialize_state(MSG_NOT_FINSIHED);
          MPI_Send(&message->data, message->length, MPI_CHAR, status.MPI_SOURCE,
            TAG_MY_FINISH_STATUS, MPI_COMM_WORLD);
          free(message);
          break;
        case TAG_FINISH:
          printf("find_solution: received request to finish from %d\n",
            status.MPI_SOURCE);
          break;
        default:
          printf("Unhandled switch branch - %d\n", status.MPI_TAG);
      }

      MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
    }
  }

  int current_solution_node_idx = list_size(current_solution) - size;
  list_node *current_solution_node = list_at_index(current_solution,
    current_solution_node_idx);
  graph_node *current_node = current_solution_node->data;

  int i;
  for (i = current_node->neighbors_count - 1; i >= 0 ; i--)
  {
    graph_node *neighbor = current_node->neighbors[i];

    if ((neighbor->visited))
    {
      continue;
    }

    list_node *new_solution = list_copy(current_solution);
    list_enque(&new_solution, neighbor);

    if (list_size(new_solution) == (2 * size))
    {
      *solutions = list_push(*solutions, new_solution);
    }
    else
    {
      if (neighbor->id != DUMMY_NODE_ID)
      {
        neighbor->visited = 1;
      }

      find_solution(solutions, new_solution, size);
      neighbor->visited = 0;
      list_free(new_solution);
    }
  }
}

void dump_solution(list_node *root)
{
  list_node *tmp = root;
  printf("%d\n", ((graph_node *) tmp->data)->id);
  tmp = tmp->next;

  int i, line_length = 1;

  while (tmp)
  {
    for (i = 0; i < line_length; i++)
    {
      printf("%02d ", ((graph_node *) tmp->data)->id);
      tmp = tmp->next;
    }
    printf("\n");
    line_length *= 2;
  }
}

void mark_nodes(list_node *root, int is_visited)
{
  list_node *tmp = root;

  while (tmp)
  {
    graph_node *node = (graph_node *) tmp->data;

    if (node->id != DUMMY_NODE_ID)
    {
      node->visited = 1;
    }

    tmp = tmp->next;
  }
}

void mark_nodes_visited(list_node *root)
{
  mark_nodes(root, 1);
}

void mark_nodes_unvisited(list_node *root)
{
  mark_nodes(root, 0);
}

double best_height = 1000.0;
list_node *best_solution;

int are_others_finished(int p)
{
  int finished_count = 0;
  MPI_Status status;
  char buffer[2];
  int i;

  for (i = 1; i < p; i++)
  {
    MPI_Send(NULL, 0, MPI_CHAR, p, TAG_ARE_YOU_FINISHED, MPI_COMM_WORLD);
  }

  for (i = 1; i < p; i++)
  {
    MPI_Recv(&buffer, MSG_LENGTH, MPI_CHAR, MPI_ANY_SOURCE,
        TAG_MY_FINISH_STATUS, MPI_COMM_WORLD, &status);

    if (message_is_finished(buffer))
    {
      finished_count += 1;
    }
  }

  return finished_count == (p - 1);
}

int _last_asked;

int last_asked(int p, int my_rank)
{
  _last_asked += 1;

  if (_last_asked == my_rank)
  {
    _last_asked += 1;
  }

  if (_last_asked >= p)
  {
    _last_asked = 0;
  }

  return _last_asked;
}

int main(int argc, char *argv[])
{
  char buffer[MSG_LENGTH];
  MPI_Status status;
  int my_rank, p;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  _last_asked = my_rank;

  struct graph *graph = graph_new_from_file(argv[1]);
  int ideal_height = ceilf(log2(graph->size)) + 1;

  list_node *solution;
  list_node *solutions;
  int repeat = 1;

  if (my_rank == 0)
  {
    solution = list_push(NULL, graph->root);
    solutions = list_push(NULL, solution);
  }
  else
  {
    MPI_Send(NULL, 0, MPI_CHAR, 0, TAG_REQUEST_DATA, MPI_COMM_WORLD);
    mpi_msg *message;

    int repeat_probe = 1;
    while (repeat_probe)
    {
      MPI_Recv(&buffer, MSG_LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
        MPI_COMM_WORLD, &status);

      switch (status.MPI_TAG)
      {
        case TAG_REQUEST_DATA:
          printf("p %d received data request\n", p);
          message = message_serialize_data(NULL);
          MPI_Send(&message->data, message->length, MPI_CHAR, status.MPI_SOURCE,
            TAG_DATA, MPI_COMM_WORLD);
          free(message);
          break;
        case TAG_DATA:
          solutions = message_deserialize_data(buffer);
          if (solutions == NULL)
          {
            MPI_Send(NULL, 0, MPI_CHAR, last_asked(p, my_rank),
              TAG_REQUEST_DATA, MPI_COMM_WORLD);
          }
          else
          {
            repeat_probe = 0;
          }
          break;
        case TAG_ARE_YOU_FINISHED:
          message = message_serialize_state(MSG_FINSIHED);
          MPI_Send(&message->data, message->length, MPI_CHAR, status.MPI_SOURCE,
            TAG_MY_FINISH_STATUS, MPI_COMM_WORLD);
          free(message);
          break;
        case TAG_FINISH:
          repeat = 0;
          repeat_probe = 0;
          break;
        default:
          printf("Unhandled switch branch - %d\n", status.MPI_TAG);
      }
    }
  }

  while (repeat)
  {
    while (list_size(solutions) > 0)
    {
      list_node *current_solution = list_pop(&solutions);
      mark_nodes_visited(current_solution);
      double height = log2(list_size(current_solution)) + 1;

      if (graph_all_visited(graph))
      {
        // TODO: Inform others about best solution.
        if (height < best_height)
        {
          best_height = height;
          best_solution = current_solution;
          if (height == ideal_height)
          {
            repeat = 0;
            break;
          }
        }
      }
      else if (height < best_height)
      {
        find_solution(&solutions, current_solution, list_size(current_solution));
      }

      mark_nodes_unvisited(current_solution);
      list_free(current_solution);
    }

    if (my_rank == 0)
    {
      if (are_others_finished(p))
      {
        repeat = 0;

        int i;
        for (i = 1; i < p; i++)
        {
          MPI_Send(NULL, 0, MPI_CHAR, p, TAG_FINISH, MPI_COMM_WORLD);
        }

        break;
      }
    }
    else
    {
      int flag = 0;
      MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);

      while (flag)
      {
        MPI_Recv(&buffer, MSG_LENGTH, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG,
            MPI_COMM_WORLD, &status);

        switch (status.MPI_TAG)
        {
          case TAG_REQUEST_DATA:
          {
            mpi_msg *message = message_serialize_data(NULL);
            MPI_Send(&message->data, message->length, MPI_CHAR,
              status.MPI_SOURCE, TAG_DATA, MPI_COMM_WORLD);
            free(message);
            break;
          }
          case TAG_DATA:
            printf("p %d received data from %d\n", my_rank, status.MPI_SOURCE);
            break;
          case TAG_ARE_YOU_FINISHED:
          {
            mpi_msg *message = message_serialize_state(MSG_FINSIHED);
            MPI_Send(&message->data, message->length, MPI_CHAR,
              status.MPI_SOURCE, TAG_MY_FINISH_STATUS, MPI_COMM_WORLD);
            free(message);
            break;
          }
          case TAG_FINISH:
            repeat = 0;
            break;
          default:
            printf("Unhandled switch branch - %d\n", status.MPI_TAG);
        }

        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &status);
      }
    }

    if (!repeat)
    {
      break;
    }

    MPI_Send(NULL, 0, MPI_CHAR, last_asked(p, my_rank), TAG_REQUEST_DATA,
      MPI_COMM_WORLD);
    MPI_Recv(&buffer, MSG_LENGTH, MPI_CHAR, 0, TAG_DATA, MPI_COMM_WORLD,
      &status);
    solutions = message_deserialize_data(buffer);
  }

  dump_solution(best_solution);
  graph_free(graph);
  MPI_Finalize();

  return 0;
}
