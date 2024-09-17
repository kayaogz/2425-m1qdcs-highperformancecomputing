#include <cstdio>

int main()
{
  printf("Hello\n");

#pragma omp parallel default(none) num_threads(3)
  {
    printf("World\n");
  }

  printf("End\n");

  return 0;
}
