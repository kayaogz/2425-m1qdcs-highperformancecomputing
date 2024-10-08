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
  // Version 1: Using omp for loop. We can collapse two loops since each iteration (i, j) is independent
  // Version 1: Avec boucle omp for. On peut faire collapse pour deux boucles car chaque iteration (i, j) est
  // independante
  // TODO / A FAIRE ...
#pragma omp parallel default(none) shared(A)
  {
#pragma omp for collapse(2) 
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      A[i][j] = i + j;
  }

  // Version 2: Using tasks, each task initializes a block of size K x K. There are (N / K) x (N / K) tasks in total
  // Version 2: Avec taches, chaque tache initialise un bloc de taille K x K. Il y a (N / K) x (N / K) taches au total
  // TODO / A FAIRE ...
#pragma omp parallel default(none) shared(A)
  {
    // We can even create tasks in parallel since they have no dependencies
    // On peut meme creer les taches en parallele car elle n'ont aucune dependence entr'eux
#pragma omp for collapse(2)
    for (int ti = 0; ti < NTASKS; ti++) {
      for (int tj = 0; tj < NTASKS; tj++) {
        // For a task, try to explicity specify the type of each variable used (except K which is a macro)
        // ti and tj cannot be shared for the task as they keep changing in the loop, so we need a private copy. A can
        // be shared as it is a constant pointer
        // Pour chaque tache, il vaut mieux explicitement preciser le type de partage (sauf K car c'est un macro)
        // ti et tj ne peuvent etre partages pour la tache car ils evoluent dans la foulee des iterations de la boucle,
        // donc on a besoin d'une copie privee a part. A peut etre partage car c'est un pointeur constant
#pragma omp task default(none) firstprivate(ti, tj) shared(A)
        for (int i = ti * K; i < (ti + 1) * K; i++) {
          for (int j = tj * K; j < (tj + 1) * K; j++) {
            A[i][j] = i + j;
          }
        }
      }
    }
  }
  if (N < 20) {
    printf("Array A:\n");
    printArray(A);
  }

  // Non-blocked version for computing B
  // Version sans bloc pour calculer B
//#pragma omp parallel default(none) shared(A, B, deps)
//  {
//#pragma omp single
//    {
//      for (int i = 0; i < N; i++) {
//        for (int j = 0; j < N; j++) {
//#pragma omp task default(none) firstprivate(i, j) shared(A, B) \
//          depend(in:A[i - 1][j - 1],A[i - 1][j],A[i][j - 1]) depend(out:A[i][j])
//          {
//            if (i == 0) { 
//              if (j == 0) {
//                B[i][j] = A[i][j];
//              } else {
//                B[i][j] = B[i][j - 1] + A[i][j];
//              }
//            } else if (j == 0) {
//              B[i][j] = B[i - 1][j] + A[i][j];
//            } else {
//              B[i][j] = B[i - 1][j] + B[i][j - 1] - B[i - 1][j - 1] + A[i][j];
//            }
//          }
//        }
//      }
//    }
//  }

  // Blocked version for computing B
  // Version avec bloc pour calculer B
#pragma omp parallel default(none) shared(A, B, deps)
  {
#pragma omp single
    {
      for (int ti = 0; ti < NTASKS; ti++) {
        for (int tj = 0; tj < NTASKS; tj++) {
#pragma omp task default(none) firstprivate(ti, tj) shared(A, B) \
          depend(out:deps[ti + 1][tj + 1]) depend(in:deps[ti][tj],deps[ti + 1][tj],deps[ti][tj + 1])
          for (int i = ti * K; i < (ti + 1) * K; i++) {
            for (int j = tj * K; j < (tj + 1) * K; j++) {
              if (i == 0) { 
                if (j == 0) {
                  B[i][j] = A[i][j];
                } else {
                  B[i][j] = B[i][j - 1] + A[i][j];
                }
              } else if (j == 0) {
                B[i][j] = B[i - 1][j] + A[i][j];
              } else {
                B[i][j] = B[i - 1][j] + B[i][j - 1] - B[i - 1][j - 1] + A[i][j];
              }
            }
          }
        }
      }
    }
  }
  if (N < 20) {
    printf("Array B:\n");
    printArray(B);
  }
  return 0;
}
