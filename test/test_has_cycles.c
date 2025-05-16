#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

int main(void) {
  graph_t gh = {0};
  graph_init(&gh, 6);

  add_edge(&gh, 0, 1, 1);
  add_edge(&gh, 1, 2, 1);
  add_edge(&gh, 2, 3, 1);
  add_edge(&gh, 3, 1, 1);

  assert(has_cycles(&gh));

  graph_clear(&gh);
  graph_init(&gh, 6);

  add_edge(&gh, 0, 1, 1);
  add_edge(&gh, 1, 2, 1);
  add_edge(&gh, 2, 3, 1);

  assert(!has_cycles(&gh));

  graph_clear(&gh);

  return 0;
}
