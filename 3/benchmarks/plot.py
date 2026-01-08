#!/usr/bin/env python3
import csv
import matplotlib.pyplot as plt
import os

os.chdir('../build')

files = ['thread1.csv', 'thread2.csv', 'thread4.csv', 'thread8.csv']
colors = ['red', 'blue', 'green', 'purple']
labels = ['1 Thread', '2 Threads', '4 Threads', '8 Threads']

plt.figure(figsize=(12, 6))

for i, (file, color, label) in enumerate(zip(files, colors, labels)):
    pieces = []
    times = []

    with open(file, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            pieces.append(int(row['NumPieces']))
            times.append(float(row['Time_ms']))

    plt.plot(pieces, times, marker='o', linewidth=2, color=color, label=label)

plt.xlabel('Number of Pieces', fontsize=12)
plt.ylabel('Time (ms)', fontsize=12)
plt.title('Chess Verdict Calculator Performance: 1 vs 2 vs 4 vs 8 Threads', fontsize=14)
plt.legend(fontsize=10)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig('benchmark_comparison.png', dpi=150)
print('Graph saved to benchmark_comparison.png')
plt.show()


