# KMeans++ with Python and C Extension

## Assignment 2 - KMeans++ for Software Project (Due Date: 25/06/2023)

This project is an implementation of the KMeans++ algorithm in Python and its integration with a C extension using Python's C API.

---

### Table of Contents

- [Introduction](#introduction)
- [Assignment Description](#assignment-description)
  - [kmeans_pp.py](#kmeans_pppy)
  - [kmeansmodule.c](#kmeansmodulec)
  - [setup.py](#setuppy)
- [Build and Running](#build-and-running)
- [Submission](#submission)
- [Optional Bonus](#optional-bonus)

---

### Introduction
The K-means++ algorithm is used for initial centroid selection for the K-means clustering algorithm. The primary goals are:
- Port existing C code into a C extension using the C API.
- Get hands-on experience with Numpy, Pandas, and other external packages.

---

### Assignment Description

#### kmeans_pp.py
This file serves as the main interface of your assignment. It includes:
- Reading user command-line arguments
- Data file reading and merging
- K-means++ implementation
- Interface with the C extension
- Outputting the results

#### kmeansmodule.c
This is a C extension for the K-means algorithm. 
- It uses the Python C API
- The API provides a function called `fit()`

#### setup.py
This file is used to build the C extension.



### Build and Running
Run the following command to build the extension:

$python3 setup.py build_ext --inplace

