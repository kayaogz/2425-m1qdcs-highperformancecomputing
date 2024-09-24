#include <iostream>
#include "omp.h"

#define N 16
#define K 4
#define NTASKS (N / K)

double A[N][N];
double B[N][N];
bool deps[NTASKS + 1][NTASKS + 1];

void printArray(double tab[N][N])
{
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      printf("%4.0lf ", tab[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main(int argc, char **argv)
{
  // Initialize the array A[i][j] = i + j in parallel.
  // Initaliser le tableau A[i][j] = i + j en parallele.
  // Version 1: Using omp for loop. We can collapse two loops since each iteration (i, j) is independent
  // Version 1: Avec boucle omp for. On peut faire collapse pour deux boucles car chaque iteration (i, j) est
  // independante
  // TODO / A FAIRE ...


  // Version 2: Using tasks, each task initializes a block of size K x K. There are (N / K) x (N / K) tasks in total
  // Version 2: Avec taches, chaque tache initialise un bloc de taille K x K. Il y a (N / K) x (N / K) taches au total
  // TODO / A FAIRE ...


  if (N < 20) {
    printf("Array A:\n");
    printArray(A);
  }

  // TODO / A FAIRE ...
  // Non-blocked version for computing B using N x N tasks
  // Version sans bloc pour calculer B avec N x N taches


  // TODO / A FAIRE ...
  // Blocked version for computing B using (N / K) x (N / K) tasks
  // Version avec bloc pour calculer B avec (N / K) x (N / K) taches


  if (N < 20) {
    printf("Array B:\n");
    printArray(B);
  }
  return 0;
}
