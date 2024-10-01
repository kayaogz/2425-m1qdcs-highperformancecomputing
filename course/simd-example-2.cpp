#include <cstdio>
#include "immintrin.h"

int main()
{
  float A[8] = {0, 2, 4, 6, 8, 10, 12, 14};
  int perm[8] = {1, 2, 3, 4, 5, 6, 7, 0};

  __m256 va;
  va = _mm256_loadu_ps(&A[0]);
  va = _mm256_set_ps(14, 12, 10, 8, 6, 4, 2, 0);
  __m256i vperm;
//  vperm = _mm256_loadu_si256((__m256i_u const *)&perm[0]);
  vperm = _mm256_set_epi32(0, 7, 6, 5, 4, 3, 2, 1);
  va = _mm256_permutevar8x32_ps(va, vperm);
  _mm256_storeu_ps(&A[0], va);

  for (int i = 0; i < 8; i++) { printf("%f ", A[i]); }
  printf("\n");

  return 0;
}
