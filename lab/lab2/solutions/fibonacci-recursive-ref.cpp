#include <cstdio>
#include <cstdlib>
#include "omp.h"

int fib(int n)
{
  // fib(0) = 0, fib(1) = 1
  if (n < 2) { return n; }
  int x, y;

  // Create a task for computing fib(n - 2). The task needs a shared access to x of the parent thread to return the
  // result. If n is too small, do not create a separate task (to avoid task creation overhead)
  // Creer une tache pour calculer fib(n - 2). La tache a besoin d'un acces partage pour x du thread parent (shared(x))
  // afin de pouvoir rendre le resultat. Si n est tres petit, ne pas creer une tache (histoire d'eviter le surcout de
  // creation de tache)
#pragma omp task shared(x) if(n > 14)
  x = fib(n - 2);

  // Here, we can create a task for fib(n - 1). However, since the parent thread will only wait afterwards, it would be
  // more optimized to not create a task, and let the parent thread compute fib(n - 1) instead.
// #pragma omp task shared(y)
  y = fib(n - 1);

  // Wait for the child task to finish before merging the results
  // Attendre la terminaison de la tache creee avant de fusionner les resultats
#pragma omp taskwait
  
  // Merge results and return
  // Fusionner les resultats et le rendre
  return x + y;
}

void printUsage(int argc, char **argv)
{
  printf("Usage: %s N\n", argv[0]);
  printf("Example: %s 13\n", argv[0]);
}

int main(int argc, char **argv)
{
  // Check the validity of command line arguments and print usage if invalid
  // Verifier la validite des arguments fournis a la ligne de commande et afficher l'utilisation si invalid
  if (argc < 2) { 
    printUsage(argc, argv);
    return 0;
  }

  // Read the index of the Fibonacci number to compute
  // Lire l'indice du nombre Fibonacci a calculer
  const int N = atoi(argv[1]);

  // Create threads, then call the function fib(N) from a single thread. fib(N) should create tasks for recursive calls,
  // which will be executed by other available threads.
  // Creer des threads, puis appeler la fonction fib(N) depuis un seul thread. fib(N) doit lui-meme creer des taches
  // pour des appels recursifs, et ces derniers seront executes par d'autres threads disponibles.
  // TODO / A FAIRE ...
  // Since N is a constant, we can make it shared
  // Comme N est une constante, on peut le rendre partage (shared(N))
#pragma omp parallel shared(N)
  {
#pragma omp single
    printf("fib[%d] = %d\n", N, fib(N));
  }

  return 0;
}
