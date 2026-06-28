# Non-Euclidean Chess Game

C++ implementation of chess on non-Euclidean surfaces with portal mechanics.

## Features

- **Multiple surfaces**: Cylindrical and Spherical geometries
- **Portal system**: Teleportation between surfaces
- **Full chess rules**: Move validation, check/checkmate detection
- **Multi-threaded verdict calculation**: Parallel checking for game state
- **Save/Load**: Game persistence

## Build

```bash
mkdir build && cd build
cmake ..
make
```

## Run

### Play Interactively
```bash
./build/cylindrical_chess_game
```

### Watch Automated Demo (recommended!)
```bash
cd build
make demo
```

Or manually:
```bash
cd build
python3 ../benchmarks/demo_portals.py
```

The demo shows:
- Standard chess moves on cylindrical surface
- **Portal teleportation** between Cylinder ↔ Sphere
- Movement on spherical surface after portal jump
- Automated gameplay with commentary

## Commands

| Command | Description |
|---------|-------------|
| `move <surf> <x1> <y1> <surf2> <x2> <y2>` | Make a move |
| `show [surface_id]` | Show board(s) |
| `portals` | List all portals |
| `save <file>` | Save game |
| `load <file>` | Load game |
| `help` | Show commands |
| `quit` | Exit |

## Testing

```bash
cd build
ctest
```

Tests include:
- Grid2D container (custom 2D graph structure)
- Surface geometries (Cylindrical, Spherical)
- Move validation
- Portal mechanics
- Verdict calculation

## Benchmarks

```bash
cd build
make benchmark
./benchmark
cd ../benchmarks
python3 plot.py
```

Generates performance graph comparing 1, 2, 4, and 8 threads for verdict calculation.

## Documentation

```bash
cd build
make docs
open docs/html/index.html
```

## Project Structure

```
3/
├── include/          # Header files
│   ├── board/       # Board, Cell, Surface classes
│   ├── pieces/      # Chess piece classes
│   ├── game/        # Game logic, Manager, History
│   ├── common/      # Enums, Move, BoardPosition
│   ├── container/   # Grid2D template container
│   └── utils/       # ThreadedVerdictCalculator
├── src/             # Implementation files
├── tests/           # Unit tests (GTest)
├── benchmarks/      # Performance tests
│   ├── benchmark.cpp       # Multi-threading benchmark
│   ├── plot.py            # Graph generator
│   └── demo_portals.py    # Automated game demo
└── CMakeLists.txt   # Build configuration
```

## Portal Mechanics

Portals are bidirectional links between surfaces:

```
Cylinder(4,4) ↔ Sphere(4,4)
Cylinder(3,5) ↔ Sphere(3,3)
```

When a piece moves onto a portal cell, it can optionally teleport to the connected surface.

Example move through portal:
```
move 0 4 3 1 4 4
```
Moves piece from Cylinder(0, 4,3) through portal to Sphere(1, 4,4).

## Architecture

- **Container**: Custom `Grid2D<T>` template class (2D graph with 8-directional neighbors)
- **Surfaces**: Abstract base with Rectangular, Cylindrical, Spherical implementations
- **Pieces**: Polymorphic chess pieces with virtual `getPossibleMoves()`
- **Game Manager**: Orchestrates game flow, validates moves, calculates verdicts
- **Multi-threading**: Parallel verdict calculation divides pieces across threads

## Requirements

- C++17
- CMake 3.10+
- GTest (for testing)
- Python 3 + matplotlib (for benchmarks/demo)
- Doxygen (optional, for documentation)
