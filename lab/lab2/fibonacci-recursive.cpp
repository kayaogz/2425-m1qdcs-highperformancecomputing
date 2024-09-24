#include <cstdio>
#include <cstdlib>
#include "omp.h"

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

  return 0;
}
