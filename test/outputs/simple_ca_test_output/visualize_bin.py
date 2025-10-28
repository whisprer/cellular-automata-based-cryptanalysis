# visualize_bin.py
import sys
import os
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.colors import LinearSegmentedColormap
import seaborn as sns

def load_binary(filename):
    with open(filename, 'rb') as f:
        data = f.read()
    return np.frombuffer(data, dtype=np.uint8)

def plot_histogram(data, title, filename):
    plt.figure(figsize=(12, 6))
    
    # Regular histogram
    plt.subplot(1, 2, 1)
    sns.histplot(data, bins=256, kde=True)
    plt.title(f"{title} - Byte Distribution")
    plt.xlabel("Byte Value")
    plt.ylabel("Frequency")
    
    # Cumulative histogram
    plt.subplot(1, 2, 2)
    sns.ecdfplot(data)
    plt.title(f"{title} - Cumulative Distribution")
    plt.xlabel("Byte Value")
    plt.ylabel("Cumulative Frequency")
    
    plt.tight_layout()
    plt.savefig(f"{filename}_histogram.png")
    print(f"Saved histogram to {filename}_histogram.png")

def plot_heatmap(data, title, filename, width=32):
    # Reshape data to 2D grid (for heatmap visualization)
    rows = len(data) // width
    if rows * width < len(data):
        rows += 1
    
    # Pad data to fit grid
    padded_data = np.zeros(rows * width, dtype=np.uint8)
    padded_data[:len(data)] = data
    
    # Reshape to grid
    grid = padded_data.reshape(rows, width)
    
    # Create heatmap
    plt.figure(figsize=(12, max(6, rows/4)))
    
    # Custom colormap from black to white
    cmap = LinearSegmentedColormap.from_list("ByteMap", ["black", "white"])
    
    sns.heatmap(grid, cmap=cmap, cbar=True)
    plt.title(f"{title} - Byte Pattern (Width={width})")
    plt.xlabel("Column")
    plt.ylabel("Row")
    
    plt.tight_layout()
    plt.savefig(f"{filename}_heatmap.png")
    print(f"Saved heatmap to {filename}_heatmap.png")

def plot_byte_correlation(data, title, filename, lag=1):
    # Calculate byte-to-byte correlation
    if len(data) <= lag:
        print(f"Warning: Data too short for lag {lag}")
        return
    
    data1 = data[:-lag]
    data2 = data[lag:]
    
    plt.figure(figsize=(8, 8))
    plt.scatter(data1, data2, alpha=0.1, s=1)
    plt.title(f"{title} - Byte Correlation (Lag={lag})")
    plt.xlabel(f"Byte[i]")
    plt.ylabel(f"Byte[i+{lag}]")
    plt.grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig(f"{filename}_correlation.png")
    print(f"Saved correlation plot to {filename}_correlation.png")

def analyze_file(filename):
    if not os.path.exists(filename):
        print(f"Error: File {filename} not found")
        return
    
    print(f"Analyzing {filename}...")
    data = load_binary(filename)
    
    # Base filename for outputs
    base_filename = os.path.splitext(filename)[0]
    title = os.path.basename(base_filename)
    
    # Generate visualizations
    plot_histogram(data, title, base_filename)
    plot_heatmap(data, title, base_filename)
    plot_byte_correlation(data, title, base_filename)
    
    # Calculate and display basic statistics
    print(f"File size: {len(data)} bytes")
    print(f"Unique byte values: {len(np.unique(data))}")
    print(f"Min byte value: {data.min()}")
    print(f"Max byte value: {data.max()}")
    print(f"Mean: {data.mean():.2f}")
    print(f"Std dev: {data.std():.2f}")
    print(f"Entropy: {calculate_entropy(data):.4f} bits per byte")
    print("\n" + "-"*50 + "\n")

def calculate_entropy(data):
    # Calculate Shannon entropy
    _, counts = np.unique(data, return_counts=True)
    probabilities = counts / len(data)
    entropy = -np.sum(probabilities * np.log2(probabilities))
    return entropy

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python visualize_bin.py <binary_file1> [binary_file2] ...")
        sys.exit(1)
    
    for filename in sys.argv[1:]:
        analyze_file(filename)