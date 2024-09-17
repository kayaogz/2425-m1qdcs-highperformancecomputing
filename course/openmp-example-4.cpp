#include <cstdio>
#include <vector>
#include "unistd.h"
#include "omp.h"


int main()
{
  int N = 10000;
  std::vector<int> A(N);

#pragma omp parallel default(none) num_threads(4) shared(A, N)
  {
    int thid = omp_get_thread_num();
    printf("thid = %d\n", thid);
#pragma omp sections
    {
#pragma omp section
      {
        printf("first half done by thid = %d\n", thid);
        for (int i = 0; i < N / 2; i++) {
          A[i] = i;
        }
      }
#pragma omp section
      {
        printf("second half done by thid = %d\n", thid);
        for (int i = N / 2; i < N; i++) {
          A[i] = i;
        }
      }
    }
  }

//  for (int i = 0; i < N; i++) {
//    printf("%d ", A[i]);
//  }
//  printf("\n");

  return 0;
}
