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

#pragma omp parallel default(none) num_threads(3) shared(two, three, four)
  {
    int thid = omp_get_thread_num();
    printf("thid = %d\n", thid);
    if (thid == 0) {
      two = computeTwo();
    } else if (thid == 1) {
      three = computeThree();
    } else if (thid == 2) {
      four = computeFour();
    }
  }

  nine = two + three + four;
  printf("nine = %d\n", nine);

  return 0;
}
