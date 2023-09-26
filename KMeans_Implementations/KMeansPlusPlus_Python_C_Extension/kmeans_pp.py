import sys
import pandas as pd
import numpy as np
import mykmeanssp

def compute_Dx(datapoints_arr, centroids,j):
    Dx = np.zeros(datapoints_arr.shape[0])
    for i in range(datapoints_arr.shape[0]):
        Dx[i] = np.min(np.sqrt(np.sum((centroids[:j, :] - datapoints_arr[i])**2, axis=1)))
    return Dx


def print_result(centroidsArray):
    for centroid in centroidsArray:
        line = ",".join("%.4f" % coord for coord in centroid)
        print(line)
    print("")



def choose_next_center(datapoints_arr, Dx):
    # print(Dx)
    # print(Dx.sum())
    probabilities = Dx / Dx.sum()
    # print(probabilities)
    random_index = np.random.choice(datapoints_arr.shape[0], p=probabilities)
    return datapoints_arr[random_index, :], int(random_index)




def main():
    # print(mykmeanssp.fit.__doc__)
    np.random.seed(0)
    user_input = check_input()
    if user_input == 1:
        return 1
    k, iterations, epsilon, first_filepath, second_filepath = user_input

    first_database = pd.read_csv(first_filepath, header=None)
    second_database = pd.read_csv(second_filepath, header=None)
    union_database = pd.merge(first_database, second_database, on=second_database.columns[0], how='inner')
    N = len(union_database)
    if k >= N or k <= 1:
        print("Invalid number of clusters!")
        return 1

    union_database = union_database.sort_values(by=union_database.columns[0])
    datapoints_arr = union_database.iloc[:, 1:].values
    

    num_of_coordinates = len(datapoints_arr[0])
    centroids_arr = np.zeros((k, num_of_coordinates))
    centroids_indexes = []
    random_index = int(np.random.choice(N))
    first_centroid = datapoints_arr[random_index]
    centroids_arr[0] = first_centroid
    centroids_indexes.append(str(random_index))

    for i in range(1,k):
        Dx = compute_Dx(datapoints_arr, centroids_arr,i)
        centroids_arr[i],new_index = choose_next_center(datapoints_arr, Dx)
        centroids_indexes.append(str(new_index))
    
    result = mykmeanssp.fit(iterations, centroids_arr, datapoints_arr, N, num_of_coordinates, k, epsilon)
    if result == None:
        return 1
    
    else:
        print(",".join(centroids_indexes))
        print_result(result)

    return 0


    
    #toDo: need to pass sorted_datapoints_arr to the C program


def check_input():
    num_of_arguments = len(sys.argv)
    if num_of_arguments != 6 and num_of_arguments != 5:
        print("An Error Has Occurred")
        return 1
    try:
        k = int(sys.argv[1])
    except:
        print("Invalid number of clusters!")
        return 1
    

    if num_of_arguments == 6:
        try:
            iterations = int(sys.argv[2])
        except:
            print("Invalid maximum iteration!")
            return 1
        if (iterations <= 1 or iterations >= 1000):
            print("Invalid maximum iteration!")
            return 1
        try:
            epsilon = float(sys.argv[3])
        except:
            print("An Error Has Occurred")
            return 1
        if epsilon < 0:
            print("An Error Has Occurred")
            return 1
        
        first_filename = sys.argv[4]
        second_filename = sys.argv[5]
    
    else:  # means num_of_argumenst = 5
        iterations = 300
        try:
            epsilon = float(sys.argv[2])
        except:
            print("An Error Has Occurred")
            return 1
        if epsilon < 0:
            print("An Error Has Occurred")
            return 1
        first_filename = sys.argv[3]
        second_filename = sys.argv[4]
    
    return [k, iterations, epsilon, first_filename, second_filename]



# def readVectors(fileName):
#     with open(fileName) as f:
#         vectors = [[float(v) for v in line.strip().split(",")] for line in f]
#     return vectors


# def sort_datapoints(datapoints_arr):
#     datapoints_arr.sort(key=lambda x: x[0])
#     for i in range(len(datapoints_arr)):
#         datapoints_arr[i] = datapoints_arr[i][1:]
#     return datapoints_arr


if __name__ == "__main__":
    main()





