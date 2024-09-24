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

#pragma omp parallel default(none) num_threads(3) shared(nine, two, three, four)
  {
    int thid = omp_get_thread_num();
    printf("thid = %d\n", thid);
#pragma omp single
    {
#pragma omp task default(none) shared(two)
      {
        two = computeTwo();
      }
#pragma omp task default(none) shared(three)
      {
        three = computeThree();
      }
#pragma omp task default(none) shared(four)
      {
        four = computeFour();
      }
#pragma omp taskwait
#pragma omp task default(none) shared(nine, two, three, four)
      {
        nine = two + three + four;
      }
    }
  }

  printf("nine = %d\n", nine);

  return 0;
}
