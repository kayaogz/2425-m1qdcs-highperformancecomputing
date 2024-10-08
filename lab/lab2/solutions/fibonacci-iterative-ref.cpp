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
  // are idle
  // Creer les threads avant de creer les taches. Seulement un thread s'occupera de la creation des taches, les autres
  // threads executerons les taches tant qu'ils sont disponibles
  // fib is an array pointer and N is a constant so we can share them among threads
  // fib est un pointer vers uu tableau et N est une constant, donc on peut les partager parmi les threads
  // TODO / A FAIRE ...
#pragma omp parallel default(none) shared(fib, N)
  {
    // Create tasks by a single thread only
    // Creer les taches par un seul thread
#pragma omp single
    for (int i = 2; i < N; i++) {
      // Create a task for computing fib[i]. The task depends on fib[i - 1] and fib[i - 2] (input dependency) and
      // modifies fib[i] (output dependency). fib is a array pointer that is constant; therefore, it can be shared. i is
      // the loop variable that keeps changing; therefore, we need to create a separate copy of the actual value for the
      // task using firstprivate(i)
      // Creer une tache pour calculer fib[i]. La tache depende de fib[i - 1] et fib[i - 2] (dependence de type input)
      // et modifie fib[i] (dependence de type output). fib est un pointer vers un tableau dont valeur est constante;
      // donc ceci peut etre partage (shared(fib)) parmi les taches. i est la variable de boucle qui evolue; donc on a
      // besoin de creer une copie a part de la valeur actuelle de i pour la tache actuelle, en utilisant
      // firstprivate(i)
#pragma omp task shared(fib) firstprivate(i) depend(in:fib[i - 1], fib[i - 2]) depend(out:fib[i])
      {
        fib[i] = fib[i - 1] + fib[i - 2];
      }
    }
  }

  // Print all computed Fibonacci numbers until n
  // Afficher tous les nombres Fibonacci jusqu'a n
  printf("Fibonacci numbers: ");
  for (int i = 0; i < N; i++) { printf("%d ", fib[i]); }
  printf("\n");
  return 0;
}
