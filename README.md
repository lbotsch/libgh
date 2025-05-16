# 📊 libgh - A Single-Header Graph Algorithms Library written in C

`gh.h` is a lightweight, easy-to-integrate single-header library for common graph algorithms written in pure C.
It is designed for performance and portability, suitable for embedding into projects without external dependencies.

## ✨ Features

- Directed and undirected graph support
- Depth-First Search (DFS)
- Breadth-First Search (BFS)
- Dijkstra's shortest path algorithm
- Topological sort
- Cycle detection
- Minimal dependencies (standard C library only)

## 📦 Getting Started

Just drop the header file into your project:

```c
#define GH_IMPLEMENTATION
#include "gh.h"

void cb(graph_t *gh, size_t node, void *user_data) {}

int main(void) {
    graph_t gh = {0};
    graph_init(&gh, 10);

    add_edge(&gh, 0, 1, 1); // 0 -0-> 1
    add_edge(&gh, 0, 2, 2); // 0 -0-> 2
    add_edge(&gh, 2, 3, 1); // 2 -0-> 3

    dfs(&gh, cb, 0, NULL);

    gh_graph_clear(&gh);
    return 0;
}
```
## 📚 Documentation

Full documentation is included as comments within the cgraph.h file. Each function is documented with usage examples.
## ✅ Requirements

- C99 or later
- No third-party libraries required

## 🧪 Testing

To build and run the test suite:

```sh
make test
```

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.
