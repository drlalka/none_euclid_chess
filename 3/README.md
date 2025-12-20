# Lab 3: Grid2D Container

2D graph template container with 8-directional links for non-Euclidean surfaces.

## Build & Test

```bash
mkdir build && cd build
cmake ..
make
make run_tests
```

## Commands

| Command | Description |
|---------|-------------|
| `make` | Build all |
| `make run_tests` | Run all tests (52 Grid2D + 2 surfaces) |
| `ctest` | Run tests via CTest |
| `./test_grid2d` | Run Grid2D tests directly |
| `./test_cylindrical` | Run surface tests directly |
| `./grid_example` | Run usage example |
| `make clean` | Clean build |
