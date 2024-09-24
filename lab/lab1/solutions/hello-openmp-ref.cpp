#include <cstdio>
#include <iostream>
#include "omp.h"

/** The order of priority for determining the number of threads is
 *  1) num_threads() clause
 *  2) omp_set_num_threads() function
 *  3) OMP_NUM_THREADS environment variable
 *
 *  L'ordre de priorite pour determiner le nombre de threads est comme la suite:
 *  1) clause num_threads()
 *  2) fonction omp_set_num_threads()
 *  3) la variable d'environnement OMP_NUM_THREADS
 */

int main()
{
  // To control the number of threads using environment variables, execute the program as follows:
  // Afin de controller le nombre de threads avec la variable d'environnement, executer le programme comme la suite:
  // OMP_NUM_THREADS=3 ./hello-openmp        // Lowest priority / Priorite basse
  omp_set_num_threads(5);                    // Medium priority / Priorite moyenne
#pragma omp parallel num_threads(7)          // High priority   / Priorite haute
  {
    printf("Thread %d/%d\n", omp_get_thread_num(), omp_get_num_threads());
#pragma omp single       // Only one (arbitrary) thread executes / Un seul thread arbitraire l'execute
//#pragma omp master       // Only thread 0 executes / Seulement le thread 0 l'execute
    printf("Hello from thread %d\n", omp_get_thread_num());
  }

  return 0;
}
