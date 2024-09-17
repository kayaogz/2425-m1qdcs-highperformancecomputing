#include <cstdio>
#include "unistd.h"
#include "omp.h"

int computeTwo()
{
  sleep(2);
  return 2;
}

int computeThree()
{
  sleep(3);
  return 3;
}

int computeFour()
{
  sleep(4);
  return 4;
}

int main()
{
  int two = 0, three = 0, four = 0, nine = 0;

#pragma omp parallel default(none) num_threads(2) shared(two, three, four)
  {
    int thid = omp_get_thread_num();
    printf("thid = %d\n", thid);
#pragma omp sections
    {
#pragma omp section
      {
        printf("four is computed by = %d\n", thid);
        four = computeFour();
      }
#pragma omp section
      {
        printf("three is computed by = %d\n", thid);
        three = computeThree();
      }
#pragma omp section
      {
        printf("two is computed by = %d\n", thid);
        two = computeTwo();
      }
    }
  }

  nine = two + three + four;
  printf("nine = %d\n", nine);

  return 0;
}
