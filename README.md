# BVH_Cpp_Sample

# Bounding Volume Hierarchy (BVH) Algorithm

## Introduction

Bounding Volume Hierarchy (BVH) is a hierarchical spatial data structure used in computer graphics and computational geometry to organize objects spatially for efficient spatial queries like ray tracing, collision detection, and visibility determination.

## BVH Algorithm

BVH algorithm constructs a binary tree where each node represents a bounding volume enclosing a subset of the objects in the scene. This tree is recursively built by splitting the set of objects into two subsets along one of the axes (usually the longest axis of the bounding box) and assigning each subset to the left and right child nodes. This process continues until each leaf node contains a small number of objects or a single object.

## About This Code

This C++ code provides a simple implementation of the BVH algorithm for organizing a set of triangles in 3D space. It includes the following main components:

- `Vector3`: A simple 3D vector class for representing points and vectors.(same like Unity's 'Vector3')
- `AABB`: Axis-Aligned Bounding Box class for representing the bounding volume of objects.
- `Triangle`: Structure representing a triangle mesh.
- `BVHNode`: Structure representing a node in the BVH tree.
- `buildBVH`: Function to construct the BVH tree recursively.
- `displayTriangle`: Function to display information about a triangle and its bounding box.
- `displayBVHNode`: Function to display information about a BVH node and its bounding box.

The `buildBVH` function takes a vector of triangles as input and recursively constructs the BVH tree. The resulting BVH tree can be used for efficient ray tracing or collision detection.

## Usage

To use this code, simply compile it using a C++ compiler(gcc) and run the executable. The code includes a simple example that constructs a set of triangles, builds the BVH tree, and displays information about the triangles and BVH nodes.

## References

- [Bounding Volume Hierarchy on Wikipedia](https://en.wikipedia.org/wiki/Bounding_volume_hierarchy)
- [BVH tree](https://zhuanlan.zhihu.com/p/430469154)
