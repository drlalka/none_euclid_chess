# Non-Euclidean Chess - CMake Scenarios

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## CMake Targets

### Game
```bash
make cylindrical_chess_game    # Build chess game
./cylindrical_chess_game       # Play game
```

### Tests
```bash
make test_grid2d                    # Grid2D container tests
make test_cylindrical               # Cylindrical surface tests
make test_iterator_detailed         # Iterator tests
make test_move_validation           # Move validation tests
make test_noneuclidean_surfaces     # Non-euclidean surface tests
make test_verdict_calculator        # Verdict calculator tests
make test_hypothetical_check        # Hypothetical check tests
```

### Run All Tests
```bash
make run_tests    # Build and run all tests
ctest             # Run tests via CTest
```

### Examples
```bash
make grid_example         # Grid2D demo
make validation_test      # Move validation demo
./grid_example
./validation_test
```

### Documentation
```bash
make docs    # Generate HTML API documentation
```

View documentation:
```bash
open build/docs/html/index.html         # macOS
xdg-open build/docs/html/index.html     # Linux
```

Requires: `brew install doxygen` (macOS) or `apt-get install doxygen` (Linux)

## Quick Start

```bash
# Build everything
mkdir build && cd build
cmake ..
make

# Play game
./cylindrical_chess_game

# Run tests
make run_tests

# Generate documentation
make docs
```

