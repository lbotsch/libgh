#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

int main(void) {
  graph_t gh = {0};
  graph_init(&gh, 10);

  add_edge(&gh, 0, 1, 1);
  assert(gh.nodes[0].size == 1);
  assert(gh.nodes[0].nodes[0] == 1);
  assert(gh.nodes[0].weights[0] == 1);

  add_edge(&gh, 0, 1, 2);
  assert(gh.nodes[0].size == 1);
  assert(gh.nodes[0].nodes[0] == 1);
  assert(gh.nodes[0].weights[0] == 2);

  add_edge(&gh, 0, 3, 1);
  assert(gh.nodes[0].size == 2);

  add_edge(&gh, 4, 5, 1);
  assert(gh.nodes[4].size == 1);

  add_edge(&gh, 5, 6, 1);
  assert(gh.nodes[5].size == 1);

  add_edge(&gh, 3, 4, 1);
  assert(gh.nodes[3].size == 1);

  //print_adjacency_list(&gh);
  graph_clear(&gh);

  return 0;
}
