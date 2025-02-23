void kernel add(global const double *M1, global const double *M2,
                global double *M3) {
  const int ID = get_global_id(0);
  const int size = get_global_size(0);

  M3[ID] = M1[ID] + M2[ID];
}

void kernel sub(global const double *M1, global const double *M2,
                global double *M3) {
  const int ID = get_global_id(0);
  const int size = get_global_size(0);

  M3[ID] = M1[ID] - M2[ID];
}

// Scalar Multiplication
void kernel mul(global double *M1, const double V) {
  const int threadID = get_global_id(0);
  const int size = get_global_size(0);

  M1[threadID] *= V;
}

// Naive General Matrix Multiplication [GeMM]
// [MxN] = [MxK] * [KxN]
void kernel gemm(global const double *M1, global const double *M2,
                 global double *M3, const int K) {
  const int M1rowID = get_global_id(0);
  const int M2colID = get_global_id(1);

  const int M = get_global_size(0);
  const int N = get_global_size(1);

  for (int k = 0; k < K; k++)
    M3[M1rowID * N + M2colID] += M1[M1rowID * K + k] * M2[k * N + M2colID];
}
void kernel cofactor(global const double *M1, global double *M2) {
  const int ID = get_global_id(0);
  const int inputID = ID * 4;
  const int sign = ID % 2 == 0 ? 1 : -1;

  M2[ID] = sign *
           (M1[inputID] * M1[inputID + 3] - M1[inputID + 1] * M1[inputID + 2]);
}

void kernel determinant(global const double *M1, global double *M2) {
  const int ID = get_global_id(0);
  const int inputID = ID * 4;

  M2[ID] = M1[inputID] * M1[inputID + 3] - M1[inputID + 1] * M1[inputID + 2];
}

void kernel transpose(global const double *M1, global double *M2) {
  const int rowID = get_global_id(0);
  const int colID = get_global_id(1);
  const int rows = get_global_size(0);
  const int cols = get_global_size(1);

  M2[colID * rows + rowID] = M1[rowID * cols + colID];
}