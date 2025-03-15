// N == Dimension
// M == Centroid Count
kernel void euclideanDistance(global const double *point,
                              global const double *centroids,
                              global int* index,
                              const int N, const int M) {
  const int id = get_global_id(0);
  const int step = get_global_size(0);
  double minDist = DBL_MAX;

  for (int m = 0; m < M; m++){ // Per Centroid
    double d = 0;
    for(int n = 0; n < N; n++) // Per Dimension
      d += point[(n * step) + id] - centroids[(n * M) + m];
    d = fabs(d);
    if(d > minDist) continue;
    minDist = d;
    index[id] = m;
  }
}

// [Cumulative,Count]
// N == Dimension
// M == Point Count
kernel void centroid(global const double* points, global const int* indexes, global double* centroids, const int N, const int M){
  const int id = get_global_id(0);
  int count = 0;

  for(int m = 0; m < M; m++){
    if(indexes[m] != id) continue;

    count++;
    for(int n = 0; n < N; n++)
      centroids[id * N + n] += points[n * M + m];
  }

  for(int n = 0; n < N; n++){
    centroids[id * N + n] /= count;
  }
}

// N == Input layer count
kernel void perceptron(global const double* inputs,
                       global const double* weights,
                       const int N,
                       global const double* biases,
                       global double* output){
const int id = get_global_id(0);

double sum = 0;
for(int i = 0; i < N; i++)
  sum += inputs[i] * weights[i];
output[id] = sum + biases[id];
}

kernel void relu(global const double* inputs, global double* outputs){
const int id = get_global_id(0);
const double EULER_NUMBER_L = 2.71828182845904523536;
outputs[id] = 1.0 / (1.0 + pow(EULER_NUMBER_L, -inputs[id]));
}