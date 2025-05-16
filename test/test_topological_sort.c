#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

int main(void) {
  graph_t gh = {0};
  graph_init(&gh, 6);

  add_edge(&gh, 0, 1, 1);
  add_edge(&gh, 1, 2, 1);
  add_edge(&gh, 1, 4, 1);
  add_edge(&gh, 2, 3, 1);
  add_edge(&gh, 2, 5, 1);

  nodelist_t l = {0};
  assert(topological_sort(&gh, &l));

  assert(l.nodes[0] == 0);
  assert(l.nodes[1] == 1);
  assert(l.nodes[2] == 4);
  assert(l.nodes[3] == 2);
  assert(l.nodes[4] == 5);
  assert(l.nodes[5] == 3);

  nodelist_free(&l);

  graph_clear(&gh);

  return 0;
}
