#include <cstdio>
#include "omp.h"

int main()
{
  int n = 0;
#pragma omp parallel for num_threads(2)
  for (int i = 0; i < 100000; i++) {
    n++;
  }
  printf("%d\n", n);

  return 0;
}
