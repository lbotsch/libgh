#include <assert.h>
#include <stdio.h>

#define GH_IMPLEMENTATION
#include "../gh.h"

#define N 10

struct user_data {
  bool visited[N];
};

void cb(graph_t *gh, size_t node, void *d) {
  UNUSED(gh);
  struct user_data *ud = d;
  ud->visited[node] = true;
}

int main(void) {
  assert(N > 0);

  graph_t gh = {0};
  graph_init(&gh, 10);

  add_edge(&gh, 0, 1, 0); // 0 -0-> 1
  add_edge(&gh, 0, 2, 0); // 0 -0-> 2
  add_edge(&gh, 2, 3, 0); // 2 -0-> 3

  add_edge(&gh, 4, 5, 0); // 4 -0-> 5

  add_edge(&gh, 6, 7, 0); // 6 -0-> 7
  add_edge(&gh, 6, 8, 0); // 6 -0-> 8

  print_adjacency_list(&gh);

  // Count the trees in the forest
  size_t start_node = 0;
  size_t tree_count = 1;
  struct user_data ud = {0};
loop:
  dfs(&gh, cb, start_node, &ud);

  // Search for an unvisited node
  for (size_t i = start_node; i < N; ++i) {
    if (!ud.visited[i]) {
      start_node = i;
      tree_count++;
      goto loop;
    }
  }

  printf("Found %ld trees in the forest\n", tree_count);
  assert(tree_count == 4);

  gh_graph_clear(&gh);
  return 0;
}
