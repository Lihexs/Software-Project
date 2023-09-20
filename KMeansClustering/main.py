import math
import sys
##test

def distance(vectorA, vectorB):
    """
    Calculate the Euclidean distance between two vectors.

    Args:
    vectorA (list): First vector.
    vectorB (list): Second vector.

    Returns:
    float: Euclidean distance between the two input vectors.
    """
    return math.sqrt(sum((a - b) ** 2 for (a, b) in zip(vectorA, vectorB)))


def findClusterIndex(centroidsArray, vector):
    """
    Find the index of the closest centroid to the given vector.

    Args:
    centroidsArray (list): List of centroids.
    vector (list): Input vector.

    Returns:
    int: Index of the closest centroid in the centroidsArray.
    """
    return min(range(len(centroidsArray)), key=lambda i: distance(centroidsArray[i], vector))


def KMeanAlgorithm(datapoints_arr, k, epsilon=0.001, iteration=200):
    """
    Perform the K-means clustering algorithm on the given dataset.

    Args:
    datapoints_arr (list): List of input data points.
    k (int): Number of clusters.
    epsilon (float, optional): Convergence threshold. Defaults to 0.001.
    iteration (int, optional): Maximum number of iterations. Defaults to 200.

    Returns:
    list: List of final centroids.
    """
    centroidsArray = datapoints_arr[:k]
    num_of_coordinates = len(centroidsArray[0])

    for _ in range(iteration):
        datapoints_num_in_centroids = [0 for _ in range(k)]
        newCentroidsArray = [[0] * num_of_coordinates for _ in range(k)]

        # Assign data points to the closest centroid
        for vector in datapoints_arr:
            index = findClusterIndex(centroidsArray, vector)
            for j in range(num_of_coordinates):
                newCentroidsArray[index][j] += vector[j]
            datapoints_num_in_centroids[index] += 1

        # Update the centroids
        for m in range(k):
            if datapoints_num_in_centroids[m] != 0:
                newCentroidsArray[m] = [coordinate / datapoints_num_in_centroids[m] for coordinate in newCentroidsArray[m]]

        # Check for convergence
        if all(distance(a, b) < epsilon for (a, b) in zip(centroidsArray, newCentroidsArray)):
            break

        centroidsArray = newCentroidsArray

    return centroidsArray


def main():
    # Parse command line arguments
    try:
        k = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        return 1

    num_of_arguments = len(sys.argv)
    if num_of_arguments == 4:
        try:
            iterations = int(sys.argv[2])
        except:
            print("Invalid maximum iteration!")
            return 1
        filename = sys.argv[3]
    else:
        filename = sys.argv[2]
        iterations = 200

    # Read data points from the input file
    with open(filename) as f:
        datapoints_arr = [[float(v) for v in line.strip().split(",")] for line in f]

    # Check if the number of clusters is valid
    if 1 < k < len(datapoints_arr):
        # Run the K-means algorithm
        centroids = KMeanAlgorithm(datapoints_arr, k, iteration=iterations)

        #
