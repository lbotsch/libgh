#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

int main(void) {
  graph_t gh = {0};
  graph_init(&gh, 6);

  add_edge(&gh, 0, 1, 2);
  add_edge(&gh, 1, 2, 6);
  add_edge(&gh, 2, 3, 3);
  add_edge(&gh, 3, 5, 10);
  add_edge(&gh, 1, 5, 11);

  size_t *path_lengths;
  shortest_path_dijkstra(&gh, 0, &path_lengths);

  assert(path_lengths[0] == 0);
  assert(path_lengths[1] == 2);
  assert(path_lengths[2] == 8);
  assert(path_lengths[3] == 11);
  assert(path_lengths[4] == UINT_MAX);
  assert(path_lengths[5] == 13);

  free(path_lengths);
  graph_clear(&gh);

  return 0;
}
