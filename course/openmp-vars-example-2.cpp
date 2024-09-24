#include <iostream>
#include "omp.h"

int globalx;
#pragma omp threadprivate(globalx)

int main()
{
#pragma omp parallel default(none) num_threads(2)
  {
    globalx = omp_get_thread_num();
    printf("%d / %d\n", globalx, omp_get_num_threads());
  }

#pragma omp parallel default(none) num_threads(2)
  {
    printf("%d / %d\n", globalx, omp_get_num_threads());
  }

  return 0;
}
