import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from sklearn.datasets import load_iris

# Load the iris dataset
data = load_iris()
X = data.data

# Create an empty list to store the inertia values
inertia_values = []

# Run k-means clustering for values of k from 1 to 10
for k in range(1, 11):
    kmeans = KMeans(n_clusters=k, init='k-means++',n_init=10, random_state=0)
    kmeans.fit(X)
    inertia_values.append(kmeans.inertia_)

# Plot the line chart of inertia values
plt.plot(range(1, 11), inertia_values, marker='o')
plt.title('Elbow Method')
plt.xlabel('Number of clusters (k)')
plt.ylabel('Inertia')

# The 'elbow' is typically at the point where the plot starts descending much more slowly.
# In the case of the iris dataset, this is often at k=3, but it may depend on the specific run.
elbow_point = 2
plt.annotate('Elbow', xy=(elbow_point, inertia_values[elbow_point-1]),
             xytext=(elbow_point+2, inertia_values[elbow_point-1]),
             arrowprops=dict(facecolor='black', shrink=0.05))

plt.savefig('elbow.png')
