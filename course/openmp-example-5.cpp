#include <cstdio>
#include <vector>
#include "unistd.h"
#include "omp.h"

#define MAX_THREADS 1000


int main()
{
  int N = 100000;
  int sum = 0;
  std::vector<int> A(N);
  std::vector<int> localSum(MAX_THREADS, 0);

#pragma omp parallel default(none) num_threads(4) shared(A, localSum, sum, N)
  {
    int thid = omp_get_thread_num();
    int numth = omp_get_num_threads();
    int elemPerThread = N / numth;

    printf("thid = %d, numth = %d\n", thid, numth);

    for (int i = thid * elemPerThread; i < (thid + 1) * elemPerThread; i++) {
//      printf("i %d, thid %d\n", i, thid);
      A[i] = i;
      localSum[thid] = localSum[thid] + A[i];
    }
#pragma omp barrier
//#pragma omp single
//    {
//      for (int i = 0; i < numth; i++) {
//        sum = sum + localSum[i];
//      }
//    } // implicit barrier
//#pragma omp critical
//    {
//      sum = sum + localSum[thid];
//    }
#pragma omp atomic
    sum = sum + localSum[thid];

//#pragma omp for reduction(+:sum)
//    for (int i = 0; i < N; i++) {
//      A[i] = i;
//      sum = sum + A[i];
//    } // implicit barrier
  }

//  for (int i = 0; i < N; i++) {
//    printf("%d ", A[i]);
//  }
  printf("\n");
//  sum = localSum[0] + localSum[1] + localSum[2] + localSum[3];
  printf("sum = %d\n", sum);

  return 0;
}
