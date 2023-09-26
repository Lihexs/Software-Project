# K-Means Clustering Algorithm Implementation in C and Python

## Introduction

This repository contains the implementation of the K-means clustering algorithm in both C and Python programming languages.

### About K-Means

K-means is a popular clustering algorithm that partitions N unlabeled observations into K distinct clusters. The algorithm works by minimizing the distance between the data points and their corresponding cluster centroids.

## Features

1. Implemented in both C and Python
2. Validates input variables
3. Accepts the number of clusters (K) and maximum number of iterations (iter) as inputs
4. Prints the final centroids

## Requirements

- C Compiler (gcc)
- Python 3.x

## How to Compile and Run

### C

Compile:

$gcc -ansi -Wall -Wextra -Werror -pedantic-errors kmeans.c -o kmeans -lm

Run example:

$./kmeans 3 100 < input_data.txt


### Python

Run example:

$python3 kmeans.py 3 100 input_data.txt




