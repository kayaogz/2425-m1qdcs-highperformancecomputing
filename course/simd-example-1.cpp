#include <cstdio>
#include "immintrin.h"

int main()
{
  int N = 1025;
  float A[N];
  float B[N];
  float C[N];

 for (int i = 0; i < N; i++) {
   A[i] = i;
   B[i] = 1;
 }
// for (int i = 0; i < N; i++) C[i] = A[i] + B[i];

  int i;
  for (i = 0; i < N - (N % 8); i = i + 8) {
    __m256 va, vb, vc;
    va = _mm256_loadu_ps(&A[i]);
    vb = _mm256_loadu_ps(&B[i]);
    vc = _mm256_add_ps(va, vb);
    _mm256_storeu_ps(&C[i], vc);
  }
  for (i = N - (N % 8); i < N; i++) { C[i] = A[i] + B[i]; } 

  for (int i = 0; i < N; i++) { printf("%f ", C[i]); }
  printf("\n");

  return 0;
}
