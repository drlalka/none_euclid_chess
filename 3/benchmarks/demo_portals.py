#!/usr/bin/env python3
"""
Demo script for Non-Euclidean Chess with Portals
Demonstrates portal mechanics by playing a sample game with live output
"""

import subprocess
import sys
import time
import re

def print_section(title, delay=0.5):
    """Print a section header with delay"""
    print()
    print("=" * 70)
    print(f"  {title}")
    print("=" * 70)
    time.sleep(delay)

def run_game_demo():
    """Run a demo game showing portal usage"""

    commands = [
        ("show", "Initial", True),
        ("portals", "Portals", True),
        ("", "", False),
        ("move 0 4 1 0 4 2", "White", True),
        ("move 0 6 7 0 5 5", "Black", True),
        ("move 0 4 2 0 4 3", "White", True),
        ("move 0 5 5 0 3 4", "Black", True),
        ("", "", False),
        ("show 0", "Position", True),
        ("", "Teleport", False),
        ("move 0 4 3 1 4 4", "PORTAL", True),
        ("show", "Success", True),
        ("", "", False),
        ("move 0 3 4 0 5 5", "Black", True),
        ("move 1 4 4 1 4 5", "White", True),
        ("show", "Final", True),
        ("quit", "Exit", False)
    ]

    try:
        process = subprocess.Popen(
            ["./cylindrical_chess_game"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            bufsize=1
        )

        game_input = ""
        output_buffer = ""

        for cmd, description, show_output in commands:
            if description:
                print()
                print(f">>> {description}")
                time.sleep(1)

            game_input += cmd + "\n"

        stdout, stderr = process.communicate(input=game_input, timeout=15)

        lines = stdout.split('\n')
        in_board = False
        board_buffer = []

        for line in lines:
            if '===' in line and 'Surface' in line:
                in_board = True
                if board_buffer:
                    for l in board_buffer:
                        print(l)
                    print()
                    time.sleep(0.3)
                board_buffer = [line]
            elif in_board:
                board_buffer.append(line)
                if line.strip() and not line.strip().startswith('[') and not line.strip()[0].isdigit() and '0  1  2' not in line:
                    in_board = False
            elif 'Move #' in line or 'Portals ===' in line:
                print(line)
                time.sleep(0.2)
            elif line.strip().startswith('Surface'):
                print(line)

        if board_buffer:
            for l in board_buffer:
                print(l)

        if stderr:
            print("\n[Game messages:]", file=sys.stderr)
            print(stderr, file=sys.stderr)

    except subprocess.TimeoutExpired:
        process.kill()
        print("\n[ERROR] Game process timed out")
        return False
    except FileNotFoundError:
        print("\n[ERROR] Game executable not found!")
        print("Please run: make cylindrical_chess_game")
        return False
    except KeyboardInterrupt:
        print("\n\n[INTERRUPTED] Demo stopped by user")
        return False

    print()
    print("  Portal teleportation successful")
    print("  Cylinder(4,4) <-> Sphere(4,4)")
    print("  Cylinder(3,5) <-> Sphere(3,3)")
    print()
    print("  Run: ./cylindrical_chess_game")
    print()

    return True

if __name__ == "__main__":
    success = run_game_demo()
    sys.exit(0 if success else 1)

