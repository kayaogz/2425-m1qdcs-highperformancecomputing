#include <cstdio>
#include "immintrin.h"

int main()
{
//  float A[8] = {0, 1, 2, 3, 4, 5, 6, 7};
//  float B[8] = {1, 1, 1, 1, 1, 1, 1, 1};
//  float C[8];

  int N = 1024;
  float A[N];
  float B[N];
  float C[N];

// for (int i = 0; i < N; i++) C[i] = A[i] + B[i];

  __m256 va, vb, vc;
  va = _mm256_loadu_ps(&A[0]);
  vb = _mm256_loadu_ps(&B[0]);
  vc = _mm256_add_ps(va, vb);
  _mm256_storeu_ps(&C[0], vc);

  for (int i = 0; i < 8; i++) { printf("%f ", C[i]); }
  printf("\n");

  return 0;
}
