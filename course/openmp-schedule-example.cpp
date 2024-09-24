#include <iostream>
#include "omp.h"
#include <vector>


int main()
{
  int N = 40;
  std::vector<int> A(N);

#pragma omp parallel num_threads(4) 
  {
#pragma omp for schedule(dynamic, 4)
    for (int i = 0; i < N; i++) {
      A[i] = i;
      printf("iter=%d, thid=%d\n", i, omp_get_thread_num());
    }
  }

  return 0;
}
