#!/usr/bin/env python3
import csv
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import os

script_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(script_dir, '../build')

files = ['thread1.csv', 'thread2.csv', 'thread4.csv', 'thread8.csv']
colors = ['red', 'blue', 'green', 'purple']
labels = ['1 Thread', '2 Threads', '4 Threads', '8 Threads']

plt.figure(figsize=(12, 6))

for i, (file, color, label) in enumerate(zip(files, colors, labels)):
    pieces = []
    times = []

    file_path = os.path.join(build_dir, file)
    with open(file_path, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            pieces.append(int(row['NumPieces']))
            times.append(float(row['Time_ms']))

    plt.plot(pieces, times, marker='o', linewidth=2, color=color, label=label)

plt.xlabel('Number of Pieces', fontsize=12)
plt.ylabel('Time (ms)', fontsize=12)
plt.title('Multithreading Performance: 1 vs 2 vs 4 vs 8 Threads (NONE verdict)', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, alpha=0.3)
plt.tight_layout()

output_path = os.path.join(script_dir, 'benchmark_comparison.png')
plt.savefig(output_path, dpi=150)
print(f'Graph saved to {output_path}')


