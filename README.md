# black_red_tree

Header-only C++20 binary search tree and red-black tree. The red-black tree locks insert and remove so several threads can share one tree.

## Layout

| File | Role |
| --- | --- |
| `treenode.hpp` | `BasetreeNode` and colored `BRTreeNode`, plus in-order and post-order walks |
| `basetree.hpp` | `BStree` and `BRtree` |
| `test/test_bs.cpp` | Random insert, search, and remove on a binary search tree |
| `test/test_br.cpp` | The same workload on a red-black tree |
| `test/test_thread.cpp` | Several threads calling search, size, insert, and remove for about 10 seconds |
| `CMakeLists.txt` | Builds the three test executables and registers them with CTest |

## Types

`BStree<T>` stores unique values of type `T`. `T` must be comparable with `<` and `>`.

| Method | Behavior |
| --- | --- |
| `empty()` / `Size()` | Whether the tree has nodes, and how many |
| `search(value)` | Returns the node, or `nullptr` if the value is absent |
| `insert(value)` | Inserts a unique value. Returns `false` if it is already present |
| `remove(value)` | Deletes the node. Returns `false` if the tree is empty or the value is missing |
| `PreNode` / `NextNode` | In-order predecessor and successor of a node |

`BRtree<T>` extends `BStree<T>` and rebalances after insert and remove so the tree stays a valid red-black tree. New nodes start red; the root is black. Insert and remove take an internal mutex. `remove` waits up to three seconds if the tree is empty.

Search and size reads on `BRtree` are not locked inside the class. `test/test_thread.cpp` wraps those calls with a `std::shared_mutex` in the test itself.

## Build and test

Requires CMake 3.20 or newer and a C++20 compiler.

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The tests print progress to stdout and exit 0. They do not assert tree invariants.

On Windows with MinGW, `.clangd` points clangd at `g++` with `--target=x86_64-w64-mingw32`.
