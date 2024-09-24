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
  bool deps[4];

#pragma omp parallel default(none) num_threads(3) shared(nine, two, three, four, deps)
  {
    int thid = omp_get_thread_num();
    printf("thid = %d\n", thid);
#pragma omp single
    {
#pragma omp task default(none) shared(two) depend(out:two) // depend(out:deps[0]) 
      {
        two = computeTwo();
      }
#pragma omp task default(none) shared(three) depend(out:three) // depend(out:deps[1]) 
      {
        three = computeThree();
      }
#pragma omp task default(none) shared(four) depend(out:four)  // depend(out:deps[2]) 
      {
        four = computeFour();
      }
#pragma omp task default(none) shared(nine, two, three, four) depend(in:two,three,four) depend(out:nine) // depend(in:deps[0],deps[1],deps[2]) depend(out:deps[3])
      {
        nine = two + three + four;
      }
    }
  }

  printf("nine = %d\n", nine);

  return 0;
}
