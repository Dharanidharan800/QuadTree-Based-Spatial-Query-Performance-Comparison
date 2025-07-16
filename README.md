# Spatial Query Optimization with QuadTree in C++

This project demonstrates the use of a **QuadTree** data structure to optimize spatial queries over a large dataset. It compares three different search strategies—**Linear Search**, **QuadTree Search**, and **Binary Search with Y-Filtering**—by evaluating their performance in querying 2D point data.

# Project Overview

- **Objective**: Efficiently perform range queries (2D rectangular) on 1 million randomly generated (x, y) points.
- **Key Features**:
  - Construction of a QuadTree index.
  - Benchmarking spatial query techniques.
  - Performance logging for comparative analysis.
  - QuadTree structure export for further analysis.


# File Structure

| File Name         | Description |
|-------------------|-------------|
| `main.cpp`        | Main source code. Handles data generation, indexing, querying, and benchmarking. |
| `quadtree_output.txt` | Output file containing serialized QuadTree structure. Auto-generated after execution. |


# Tools and Appoarch

- **Language**: C++
- **Compiler**: g++ or any C++11 compatible compiler
- **Concepts Used**:
  - QuadTree for spatial indexing
  - Range Query optimization
  - Random data generation
  - Benchmarking with `chrono` library

# Prerequisites

- C++11 or higher
- g++ installed

# Working 
Upon execution, the program:
- Generates 1 million random (x, y) points.
- Builds a QuadTree.
- Executes 20 queries (random or custom).
- Compares and prints query times for:
  - Linear Search
  - QuadTree Search
  - Binary Search + Y-axis Filter


## 📊 Output Sample

```
Query 1: x1=221, y1=380, x2=271, y2=430
Linear:         1186, Time = 942 µs
QuadTree:       1186, Time = 112 µs
Binary+Y-Filter:1186, Time = 137 µs
```

# How It Works

# Data Generation
- 1 million random points in the range `[0, 1000] x [0, 1000]`.

# QuadTree Construction
- Each node has a capacity (`50` points).
- Nodes subdivide into four quadrants when full.
- Recursion continues until each point lies within a node or sub-node.

# Spatial Querying
Each query defines a rectangular region:
- **Linear Search**: Iterates over all points.
- **QuadTree**: Traverses relevant nodes only.
- **Binary Search + Y-Filter**: Narrows down candidates by x-coordinate (binary search) and filters by y.

# Saving Results
- The QuadTree structure is saved to `quadtree_output.txt`.


# Customization

- To switch to **custom queries**, uncomment the `customQueries` block in `main.cpp`.
- To change QuadTree capacity or boundary size, modify parameters in the `QuadTreeNode` constructor.


# Performance Insights

| Method                | Strengths                          | Weaknesses                          |
|-----------------------|------------------------------------|-------------------------------------|
| Linear Search         | Simple, easy to implement          | Slow for large datasets             |
| QuadTree Search       | Very fast for localized queries    | Slightly more memory usage          |
| Binary + Y Filtering  | Efficient when x-coordinates are sorted | Not effective on unsorted data |


# Use Cases

- Geospatial databases
- GIS systems
- Game development (collision detection, visibility)
- Real-time spatial analytics

