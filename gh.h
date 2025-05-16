

#ifndef GHI_INCLUDE_GH_H
#define GHI_INCLUDE_GH_H

#define GH_VERSION 1

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GHIDEF
#ifdef GH_STATIC
#define GHIDEF static
#else
#define GHIDEF extern
#endif
#endif

#ifndef GHPREFIX
#define GHPREFIX
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef UNUSED
#define UNUSED(x) (void)(x)
#endif

////////////////////////////////////////////////////////////////////////////////
//
// PRIMARY API
//

typedef struct {
  size_t size;
  size_t capacity;

  size_t *nodes;
  size_t *weights;
} gh_nodelist_t;

typedef struct {
  size_t size;
  size_t capacity;

  gh_nodelist_t *nodes;
} gh_graph_t;

//
// Initialize a graph containing N nodes
//
GHIDEF void gh_graph_init(gh_graph_t *gh, size_t N);
//
// Reset a graph to its uninitialized state
//
GHIDEF void gh_graph_clear(gh_graph_t *gh);
//
// Append a node to the nodelist
//
GHIDEF void gh_nodelist_append(gh_nodelist_t *l, size_t node, size_t weight);
//
// Free the nodelist
//
GHIDEF void gh_nodelist_free(gh_nodelist_t *l);
//
// Add an edge from nodes i to j with a weight w to the graph.
//
GHIDEF void gh_add_edge(gh_graph_t *gh, size_t i, size_t j, int w);
//
// Print the adjacency list of the graph.
//
GHIDEF void gh_print_adjacency_list(gh_graph_t *gh);

/////////////////// ALGORITHMS ///////////////////////

typedef void (*traversal_cb)(gh_graph_t *gh, size_t node, void *arg);

//
// Calls cb(gh, n, arg) for each node n in BFS-order
// O(E+V) Time, O(V) Space
//
GHIDEF void gh_bfs(gh_graph_t *gh, traversal_cb cb, size_t start_node,
                   void *arg);

//
// Calls cb(gh, n, arg) for each node n in DFS-order
// O(E+V) Time, O(V) Space
//
GHIDEF void gh_dfs(gh_graph_t *gh, traversal_cb cb, size_t start_node,
                   void *arg);

//
// Topological sort
// O(E+V) Time, O(V) Space
//
GHIDEF bool gh_topological_sort(gh_graph_t *gh, gh_nodelist_t *nodes);

//
// Detect cycles in a graph
// O(E+V) Time, O(V) Space
//
GHIDEF bool gh_has_cycles(gh_graph_t *gh);

//
// Find the shortest path between node i and all other nodes in the graph
// O(E*logV) Time, O(V) Space
//
GHIDEF void gh_shortest_path_dijkstra(gh_graph_t *gh, size_t i,
                                      size_t *path_lengths[]);

#ifdef __cplusplus
}
#endif

////////////////////////////////////////////////////////////////////////////////
#endif // GHI_INCLUDE_GH_H

#ifdef GH_IMPLEMENTATION

#ifndef GH_ASSERT
#include <assert.h>
#define GH_ASSERT assert
#endif

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void gh_graph_init(gh_graph_t *gh, size_t N) {
  size_t cap = 64u;
  while (cap < N)
    cap *= 2u;
  gh->size = N;
  gh->capacity = cap;
  gh->nodes = malloc(cap * sizeof(gh_nodelist_t));
  GH_ASSERT(gh->nodes != NULL);
  memset(gh->nodes, 0, N * sizeof(gh_nodelist_t));
}

void gh_graph_clear(gh_graph_t *gh) {
  for (size_t i = 0; i < gh->size; ++i) {
    if (gh->nodes[i].capacity > 0) {
      gh_nodelist_free(&gh->nodes[i]);
    }
  }
  gh->size = 0u;
  gh->capacity = 0u;
  free(gh->nodes);
}

void gh_nodelist_append(gh_nodelist_t *l, size_t node, size_t weight) {
  // Resize the nodelist if needed
  if (l->size >= l->capacity) {
    l->capacity = l->capacity == 0 ? 256 : 2u * l->capacity;
    l->nodes = realloc(l->nodes, l->capacity * sizeof(size_t));
    l->weights = realloc(l->weights, l->capacity * sizeof(size_t));
  }
  l->nodes[l->size] = node;
  l->weights[l->size] = weight;
  l->size++;
}

void gh_nodelist_free(gh_nodelist_t *l) {
  free(l->nodes);
  free(l->weights);
  l->size = 0;
  l->capacity = 0;
}

void gh_add_edge(gh_graph_t *gh, size_t i, size_t j, int w) {
  GH_ASSERT(i < gh->size);
  GH_ASSERT(j < gh->size);
  gh_nodelist_t *l = &gh->nodes[i];

  // Check if the edge already exists
  for (size_t jj; jj < l->size; ++jj) {
    if (l->nodes[jj] == j) {
      l->weights[jj] = w;
      return;
    }
  }

  gh_nodelist_append(l, j, w);
}

void gh_print_adjacency_list(gh_graph_t *gh) {
  printf("Adjacency list (N = %ld)\n", gh->size);
  for (size_t i = 0; i < gh->size; ++i) {
    printf("%ld -> (", i);
    for (size_t j = 0; j < gh->nodes[i].size; ++j) {
      if (j > 0)
        printf(", ");
      printf("%ld[%ld]", gh->nodes[i].nodes[j], gh->nodes[i].weights[j]);
    }
    printf(")\n");
  }
}

///////// Utility Data Structures ///////////

struct gh_queue {
  size_t front;
  size_t size;
  size_t capacity;

  size_t *distances;
  size_t *nodes;
};
static void gh_queue_push(struct gh_queue *q, size_t dist, size_t node) {
  if (q->size >= q->capacity) {
    q->capacity = q->capacity == 0 ? 256 : 2u * q->capacity;
    q->nodes = realloc(q->nodes, q->capacity * sizeof(size_t));
    q->distances = realloc(q->distances, q->capacity * sizeof(size_t));
  }
  q->distances[q->size] = dist;
  q->nodes[q->size] = node;
  q->size++;
}
static void gh_queue_pop_front(struct gh_queue *q, size_t *dist, size_t *node) {
  assert(q->size > q->front);

  if (dist != NULL)
    *dist = q->distances[q->front];
  if (node != NULL)
    *node = q->nodes[q->front];
  q->front++;
}
static void gh_queue_pop(struct gh_queue *q, size_t *dist, size_t *node) {
  assert(q->size > 0);

  q->size--;
  if (dist != NULL)
    *dist = q->distances[q->size];
  if (node != NULL)
    *node = q->nodes[q->size];
}
static bool gh_queue_empty(struct gh_queue *q) { return q->size <= q->front; }
static void gh_queue_free(struct gh_queue *q) {
  free(q->distances);
  free(q->nodes);
}

struct gh_minheap {
  size_t size;
  size_t capacity;

  size_t *nodes;
  size_t *weights;
};
static void gh_minheap_swap(struct gh_minheap *h, size_t i, size_t j) {
  size_t node = h->nodes[i];
  size_t weight = h->weights[i];
  h->nodes[i] = h->nodes[j];
  h->weights[i] = h->weights[j];
  h->nodes[j] = node;
  h->weights[j] = weight;
}
static void gh_minheap_heapify(struct gh_minheap *h, size_t i) {
  size_t smallest = i;
  while (true) {
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;
    if (left < h->size && h->weights[left] < h->weights[smallest])
      smallest = left;
    if (right < h->size && h->weights[right] < h->weights[smallest])
      smallest = right;
    if (smallest != i) {
      gh_minheap_swap(h, i, smallest);
      i = smallest;
      continue;
    }
    break;
  }
}
static void gh_minheap_insert(struct gh_minheap *h, size_t node,
                              size_t weight) {
  if (h->size >= h->capacity) {
    h->capacity = h->capacity == 0 ? 256 : 2 * h->capacity;
    h->nodes = realloc(h->nodes, h->capacity * sizeof(size_t));
    h->weights = realloc(h->weights, h->capacity * sizeof(size_t));
  }
  size_t i = h->size++;
  h->nodes[i] = node;
  h->weights[i] = weight;

  while (i != 0 && h->weights[(i - 1) / 2] > h->weights[i]) {
    gh_minheap_swap(h, i, (i - 1) / 2);
  }
}
static void gh_minheap_extract(struct gh_minheap *h, size_t *node,
                               size_t *weight) {
  assert(h->size > 0);

  *node = h->nodes[0];
  *weight = h->weights[0];

  h->size--;
  if (h->size == 0)
    return;

  h->nodes[0] = h->nodes[h->size];
  h->weights[0] = h->weights[h->size];
  gh_minheap_heapify(h, 0);
}
static void gh_minheap_free(struct gh_minheap *h) {
  h->size = 0;
  h->capacity = 0;
  free(h->nodes);
  free(h->weights);
}

////////////////// ALGORITHMS //////////////////////

void gh_bfs(gh_graph_t *gh, traversal_cb cb, size_t start_node, void *arg) {
  GH_ASSERT(gh->size > start_node);

  bool visited[gh->size] = {};
  struct gh_queue q = {0};

  gh_queue_push(&q, 0, start_node);
  while (!gh_queue_empty(&q)) {
    size_t n;
    gh_queue_pop_front(&q, NULL, &n);
    cb(gh, n, arg);

    for (size_t i = 0; i < gh->nodes[n].size; ++i) {
      size_t v = gh->nodes[n].nodes[i];
      if (!visited[v]) {
        visited[n] = true;
        gh_queue_push(&q, 0, v);
      }
    }
  }
  gh_queue_free(&q);
}
void gh_dfs(gh_graph_t *gh, traversal_cb cb, size_t start_node, void *arg) {
  GH_ASSERT(gh->size > start_node);

  bool visited[gh->size] = {};
  struct gh_queue q = {0};

  gh_queue_push(&q, 0, start_node);
  while (!gh_queue_empty(&q)) {
    size_t n;
    gh_queue_pop(&q, NULL, &n);
    if (visited[n])
      continue;
    visited[n] = true;
    cb(gh, n, arg);

    for (int i = gh->nodes[n].size - 1; i >= 0; --i) {
      size_t v = gh->nodes[n].nodes[i];
      if (!visited[v])
        gh_queue_push(&q, 0, v);
    }
  }
  gh_queue_free(&q);
}

bool gh_topological_sort(gh_graph_t *gh, gh_nodelist_t *nodes) {
  size_t indegree[gh->size] = {};
  struct gh_queue q = {0};
  size_t visited = 0;

  for (size_t i = 0; i < gh->size; ++i)
    for (size_t j = 0; j < gh->nodes[i].size; ++j)
      indegree[gh->nodes[i].nodes[j]]++;
  for (size_t i = 0; i < gh->size; ++i)
    if (indegree[i] == 0)
      gh_queue_push(&q, i, i);

  // Topological sort
  while (!gh_queue_empty(&q)) {
    size_t u;
    gh_queue_pop(&q, NULL, &u);

    if (nodes != NULL)
      gh_nodelist_append(nodes, u, 0);
    visited++;

    for (size_t j = 0; j < gh->nodes[u].size; ++j) {
      size_t v = gh->nodes[u].nodes[j];
      indegree[v]--;
      if (indegree[v] == 0) {
        gh_queue_push(&q, v, v);
      }
    }
  }
  gh_queue_free(&q);

  return visited == gh->size;
}

bool gh_has_cycles(gh_graph_t *gh) { return !gh_topological_sort(gh, NULL); }

void gh_shortest_path_dijkstra(gh_graph_t *gh, size_t i,
                               size_t *path_lengths[]) {
  GH_ASSERT(i < gh->size);

  size_t *dist = malloc(gh->size * sizeof(size_t));
  struct gh_minheap h;

  for (size_t k = 0; k < gh->size; ++k)
    dist[k] = UINT_MAX;
  dist[i] = 0;
  gh_minheap_insert(&h, i, 0);

  while (h.size > 0) {
    size_t d1;
    size_t n1;
    gh_minheap_extract(&h, &n1, &d1);
    for (size_t k = 0; k < gh->nodes[n1].size; ++k) {
      size_t n2 = gh->nodes[n1].nodes[k];
      size_t d2 = d1 + gh->nodes[n1].weights[k];
      if (dist[n2] > d2) {
        dist[n2] = d2;
        gh_minheap_insert(&h, n2, d2);
      }
    }
  }
  gh_minheap_free(&h);
  *path_lengths = dist;
}

#endif // GH_IMPLEMENTATION

#ifndef GH_NOSTRIP_PREFIX
#ifndef GH_STRIP_PREFIX_GUARD
#define GH_STRIP_PREFIX_GUARD

#define nodelist_t gh_nodelist_t
#define graph_t gh_graph_t
#define graph_init gh_graph_init
#define graph_clear gh_graph_clear
#define nodelist_append gh_nodelist_append
#define nodelist_free gh_nodelist_free
#define add_edge gh_add_edge
#define print_adjacency_list gh_print_adjacency_list
#define bfs gh_bfs
#define dfs gh_dfs
#define topological_sort gh_topological_sort
#define has_cycles gh_has_cycles
#define shortest_path_dijkstra gh_shortest_path_dijkstra

#endif // GH_STRIP_PREFIX_GUARD
#endif //
