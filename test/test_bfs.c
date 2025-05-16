#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

struct data {
  size_t nodes[6];
  int counter;
};

static void bfs_cb(graph_t *gh, size_t node, void *data) {
  UNUSED(gh);
  struct data *d = (struct data *)data;
  d->nodes[d->counter++] = node;
}

int main(void) {
  graph_t gh = {0};
  graph_init(&gh, 6);

  struct data d = {0};

  add_edge(&gh, 0, 1, 1);
  add_edge(&gh, 0, 2, 1);
  add_edge(&gh, 2, 3, 1);
  add_edge(&gh, 2, 4, 1);
  add_edge(&gh, 4, 5, 1);

  bfs(&gh, bfs_cb, 0, &d);
  assert(d.nodes[0] == 0);
  assert(d.nodes[1] == 1);
  assert(d.nodes[2] == 2);
  assert(d.nodes[3] == 3);
  assert(d.nodes[4] == 4);
  assert(d.nodes[5] == 5);

  graph_clear(&gh);

  return 0;
}
