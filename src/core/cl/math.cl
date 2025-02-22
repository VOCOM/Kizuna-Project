void kernel add(global const int *V1, global const int *V2, global int *V3,
                const int N) {
  const int threadID = get_global_id(0);
  const int size = get_global_size(0);
  int step = (N / size);

  for (int i = threadID; i < N; i += step)
    V3[i] = V1[i] + V2[i];
}

void kernel sub(global const int *V1, global const int *V2, global int *V3,
                const int N) {
  const int threadID = get_global_id(0);
  const int threadCount = get_global_size(0);
  int step = (N / threadCount);

  for (int i = threadID; i < N; i += step)
    V3[i] = V1[i] - V2[i];
}

// Naive General Matrix Multiplication [GeMM]
// [MxN] = [MxK] * [KxN]
void kernel mul(const int M, const int N, const int K, global const int *M1,
                global const int *M2, global int *M3) {
  const int rowID = get_global_id(0);
  const int colID = get_global_id(1);

  for (int k = 0; k < K; k++)
    M3[rowID + colID * M] += M1[k * M + rowID] * M2[k + colID * K];
}

void kernel transpose(global const int *M1, global int *M2) {
  const int rowID = get_global_id(0);
  const int colID = get_global_id(1);
  const int rows = get_global_size(0);
  const int cols = get_global_size(1);

  // printf("Cell [%d, %d]\n", rowID, colID);
  M2[colID * rows + rowID] = M1[rowID * cols + colID];
}