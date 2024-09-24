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

  // Allocate and initialize the array containing Fibonacci numbers
  // Allouer et initialiser le tableau contenant les nombres de Fibonacci
  int fib[1000];
  fib[0] = 0;
  fib[1] = 1;

  // Create threads before creating tasks. Only one thread will create tasks, other threads will execute them when they
  // are idle. fib is an array pointer and N is a constant so we can share them among threads.
  // Creer les threads avant de creer les taches. Seulement un thread s'occupera de la creation des taches, les autres
  // threads executerons les taches tant qu'ils sont disponibles.
  // TODO / A FAIRE ...

  // Print all computed Fibonacci numbers until n
  // Afficher tous les nombres Fibonacci jusqu'a n
  printf("Fibonacci numbers: ");
  for (int i = 0; i < N; i++) { printf("%d ", fib[i]); }
  printf("\n");
  return 0;
}
