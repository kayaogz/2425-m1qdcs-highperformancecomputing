#include <iostream>
#include "omp.h"

int main()
{
  int x = -1;
#pragma omp parallel num_threads(2) firstprivate(x)
  {
    printf("%d\n", x);
#pragma omp barrier
    x = omp_get_thread_num();
    printf("%d / %d\n", x, omp_get_num_threads());
  }

  printf("master threads x %d\n", x);

  return 0;
}
