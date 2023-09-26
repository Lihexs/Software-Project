#define PY_SSIZE_T_CLEAN
#include <Python.h>


int min_centroid(double ** centroid_arr, double * datapoint, int num_of_coordinates, int K);
double euclidean_distance(double *datapoint1, double *datapoint2,  int num_of_coordinates);
int set_arguments(PyObject* inputList1, PyObject* inputList2,int iterations, double** centroids_arr, double** datapoints_arr, int K, int num_of_coordinates, int num_of_datapoints);
static PyObject* fit(PyObject *self, PyObject *args);
int contains_only_digits(char * word);
int min_centroid(double ** centroid_arr, double * datapoint, int num_of_coordinates, int K);







int set_arguments(PyObject* inputList1, PyObject* inputList2,int iterations, double** centroids_arr, double** datapoints_arr, int K, int num_of_coordinates, int num_of_datapoints){
    Py_ssize_t i, j;
    PyObject* sublist;

    // centroids_arr = (double**)malloc(*K * sizeof(double*));
    for (i=0; i< K; i++){
        centroids_arr[i] = (double*)malloc(num_of_coordinates * sizeof(double));
        sublist = PySequence_GetItem(inputList1, i);
        for (j=0; j < num_of_coordinates; j++){
            centroids_arr[i][j] = PyFloat_AsDouble(PySequence_GetItem(sublist, j));
        }
    }

    // datapoints_arr = (double**)malloc(*num_of_datapoints * sizeof(double*));
    for(i=0; i < num_of_datapoints; i++){
        datapoints_arr[i] = (double*)malloc(num_of_coordinates * sizeof(double));
        sublist = PySequence_GetItem(inputList2, i);
        for (j=0; j < num_of_coordinates; j++){
            datapoints_arr[i][j] = PyFloat_AsDouble(PySequence_GetItem(sublist, j));
        }
    }
    
    return 0;
}




int min_centroid(double ** centroid_arr, double * datapoint, int num_of_coordinates, int K){
    double tmp_min_distance;
    double min_distance = euclidean_distance(centroid_arr[0], datapoint, num_of_coordinates);
    int index_of_min_centroid = 0;
    int i;
    for (i = 1; i < K; i++){
        tmp_min_distance = euclidean_distance(centroid_arr[i], datapoint, num_of_coordinates);
        if (tmp_min_distance < min_distance){
            min_distance = tmp_min_distance;
            index_of_min_centroid = i;
        }
    }
    return index_of_min_centroid;
}


double euclidean_distance(double *datapoint1, double *datapoint2, int num_of_coordinates){
   double sum = 0;
   int i;
   for (i = 0; i < num_of_coordinates; i++){
       sum +=  pow((datapoint1[i] - datapoint2[i]), 2);
   }
   return sqrt(sum);
}


int contains_only_digits(char * word){
    int i = 0;
    int num_of_dots = 0;
    int is_only_digits = 1;
    while (word[i] != '\0'){
        if ((word[i] < '0' || word[i] > '9')){
            if (word[i] == '.'){
                if (num_of_dots == 0){
                    num_of_dots++;
                }
                else{
                    is_only_digits = 0;
                } 
            }
            else{
                is_only_digits = 0;
            }
            
        }
        i++;
    }
    return is_only_digits;
}


int kmeans_algoritm(int iterations, double*** centroids_arr_ptr, double** datapoints_arr,int num_of_datapoints, int num_of_coordinates, int K, double epsilon){
    int i = 0;
    int j = 0;
    int m = 0;
    double** centroids_arr = *centroids_arr_ptr;
    int smaller_than_epsilon = 1;
    double ** new_centroids_arr;
    int index_of_centroid;
    int *datapoints_num_in_centroids;

    datapoints_num_in_centroids = calloc(K, sizeof(int));
    if (datapoints_num_in_centroids == NULL){
        return 0;
    }
    while (iterations){
        
        new_centroids_arr = (double**) malloc(K * sizeof (double *));
        if (new_centroids_arr == NULL){
            return 0;
        }
        for(i=0; i < K; i++){
            new_centroids_arr[i] = (double*) calloc(num_of_coordinates,sizeof(double));
            if (new_centroids_arr[i] == NULL){
                return 0;
            }
        }
        for (i=0; i < num_of_datapoints; i++){
            index_of_centroid = min_centroid(centroids_arr, datapoints_arr[i], num_of_coordinates, K);
            for (j=0; j < num_of_coordinates; j++){
                new_centroids_arr[index_of_centroid][j] += datapoints_arr[i][j];
            }
            datapoints_num_in_centroids[index_of_centroid] += 1;
        }

        for (i=0; i < K; i++){
            for (j=0; j < num_of_coordinates; j++){
                if (datapoints_num_in_centroids[i] != 0){
                    new_centroids_arr[i][j] = new_centroids_arr[i][j] / datapoints_num_in_centroids[i];                  
                } else {
                    new_centroids_arr[i][j] = centroids_arr[i][j];
                }
            }
        }

        for (i=0; i < K; i++){
            if (euclidean_distance(new_centroids_arr[i], centroids_arr[i], num_of_coordinates) >= epsilon){
               smaller_than_epsilon = 0;
               for(m=0; m < K; m++){
                datapoints_num_in_centroids[m] = 0;
               }
               for(j = 0; j < K; j++){
                free(centroids_arr[j]);
            }
            free(centroids_arr);
            centroids_arr = new_centroids_arr;
            new_centroids_arr = NULL;
               break;
            }
        }
        if (smaller_than_epsilon){
            break;
        }
        smaller_than_epsilon = 1;
        iterations--;
    }

    if (new_centroids_arr != NULL){
        *centroids_arr_ptr = new_centroids_arr;
           for(j = 0; j < K; j++){
                free(centroids_arr[j]);
            }
            free(centroids_arr);
    }
    else{
        *centroids_arr_ptr = centroids_arr;
    }
    free(datapoints_num_in_centroids);
    for (i = 0; i < num_of_datapoints; i++) {
        free(datapoints_arr[i]);
    }
    free(datapoints_arr);

    //free_arrays(iterations, num_of_datapoints, centroids_arr, new_centroids_arr, K, datapoints_num_in_centroids, datapoints_arr);
    return 1;
}





static PyObject* fit(PyObject *self, PyObject *args){ /* wrapper function*/
    int iterations;
    double** centroids_arr;
    double** datapoints_arr;
    int num_of_datapoints;
    int num_of_coordinates;
    int K;
    double epsilon;
    PyObject* inputList1;
    PyObject* inputList2;
    int i;
    int j;
    PyObject* result;

     if (!PyArg_ParseTuple(args, "iOOiiid", &iterations, &inputList1, &inputList2, &num_of_datapoints, &num_of_coordinates, &K, &epsilon)) {
        return NULL;  // Parsing failed, return an error
    }
    // if (PySequence_Check(inputList1)){
    //     printf("success");
    // }
    centroids_arr = (double**)malloc(K * sizeof(double*));
    datapoints_arr = (double**)malloc(num_of_datapoints * sizeof(double*));

    if (set_arguments(inputList1, inputList2, iterations, centroids_arr, datapoints_arr, K, num_of_coordinates, num_of_datapoints) == 1){
        return NULL;
    }

    kmeans_algoritm(iterations, &centroids_arr, datapoints_arr, num_of_datapoints, num_of_coordinates, K, epsilon);

    result = PyList_New(K);
     if (result == NULL) {
        printf("An Error Has Occurred");
        // Handle the case when creating the list object fails
        return NULL;
    }

    for (i = 0; i < K; i++) {
        PyObject* innerList = PyList_New(num_of_coordinates);
        if (innerList == NULL) {
            printf("An Error Has Occurred");
            // Handle the case when creating the inner list object fails
            Py_DECREF(result);
            return NULL;
        }

    for (j = 0; j < num_of_coordinates; j++) {
            PyObject* floatObj = PyFloat_FromDouble(centroids_arr[i][j]);
            if (floatObj == NULL) {
                printf("An Error Has Occurred");
                // Handle the case when creating the float object fails
                Py_DECREF(innerList);
                Py_DECREF(result);
                return NULL;
            }

            PyList_SET_ITEM(innerList, j, floatObj);
        }

        PyList_SET_ITEM(result, i, innerList);
    }

    for(j = 0; j < K; j++){
        free(centroids_arr[j]);
    }
    free(centroids_arr);
    return result;
    }

    
    




static PyMethodDef kmeansMethods[] = {
    {"fit",
    (PyCFunction) fit,
    METH_VARARGS,
    PyDoc_STR("fit function arguments:\n"
          "argument 1: num_of_iterations for the kmeans algorithm\n"
          "argument 2: the initialized centroids, as a numpy array\n"
          "argument 3: all the datapoints, as a numpy array\n"
          "argument 4: the number of datapoints\n"
          "argument 5: the number of coordinates of every datapoint\n"
          "argument 6: the number of clusters\n"
          "argument 7: epsilon\n")},
    {NULL, NULL, 0, NULL}
};



static struct PyModuleDef mykmeansmodule = {
    PyModuleDef_HEAD_INIT,
    "mykmeanssp", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1,  /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    kmeansMethods /* the PyMethodDef array from before containing the methods of the extension */
};


PyMODINIT_FUNC PyInit_mykmeanssp(void)
{
    PyObject *m;
    m = PyModule_Create(&mykmeansmodule);
    if (!m) {
        return NULL;
    }
    return m;
}
