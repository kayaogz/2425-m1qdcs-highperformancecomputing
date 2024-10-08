#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <chrono>
#include "omp.h"

#define SWAP(a, b) { auto tmp = a; a = b; b = tmp; }
#define SIZE 1024

/**
 * Verify if the given array is sorted
 * Verifier si le tableau donne est trie
 */
bool isSorted(int *a, int size)
{
  bool sorted = true;
#pragma omp parallel default(none) shared(a, size, sorted)
  {
#pragma omp for reduction (&:sorted)
    for (int i = 0; i < size - 1; i++) sorted &= (a[i] <= a[i + 1]);
  }

  return sorted;
}

/**
 * Merge two sorted subarrays a[0...size/2 - 1] and a[size/2...size - 1] into a single sorted array a[0...size - 1].
 * Use temp as a temporary buffer for merge
 * Fusionner deux sous tableaux tries a[0...size/2 - 1] et a[size/2...size - 1] en un seul tableau trie a[0...size -
 * 1]. Utiliser temp comme un buffer temporaire pour effectuer le merge
 */
void merge(int* a, int size, int* temp)
{
  int i1 = 0;
  int i2 = size / 2;
  int it = 0;

  // Do the merge on temp
  // Effectuer la fusion sur temp
  while(i1 < size / 2 && i2 < size) {
    if (a[i1] <= a[i2]) {
      temp[it] = a[i1];
      i1 += 1;
    }
    else {
      temp[it] = a[i2];
      i2 += 1;
    }
    it += 1;
  }

  while (i1 < size / 2) {
    temp[it] = a[i1];
    i1++;
    it++;
  }
  while (i2 < size) {
    temp[it] = a[i2];
    i2++;
    it++;
  }

  // Copy temp back into a
  // Recopier temp dans a
  memcpy(a, temp, size * sizeof(int));
}

/**
  * Sort the array a with size elements, using temp as a temporary merge buffer
  * Trier le tableau a avec size elements, en utilisant temp comme buffer temporaire pour la fusion
  */ 
void mergesort(int *a, int size, int *temp)
{
  if (size < 2) { return; }  // Nothing to sort / Rien a trier
  if (size == 2) {           // Only two elements to sort / Seulement deux elements a trier
    if (a[0] <= a[1])
      return;
    else {
      SWAP(a[0], a[1]);
      return;
    }
  } else {
    // Perform mergesort on each half array. Parallelize using tasks
    // Faire mergesort dans chaque moitie du tableau. Paralleliser avec taches
    // TODO / A FAIRE ...
    // Create a task to sort the first half. If the array is too small, do not create a task to avoid the overhead
    // Creer une tache pour trier la moitie. Si le tableau est petit, ne pas creer de tache pour eviter le surcout
#pragma omp task if(size > 1000) 
    mergesort(a, size/2, temp); // first size / 2 elements / les premiers size / 2 elements

    // It is better not to do this in a separate task, use the current thread instead
    // Ce serait mieux de ne pas faire ceci dans une tache a part; utiliser le thread actuel en lieu et place
//#pragma omp task if(size > 1000) 
    mergesort(a + size/2, size - size/2, temp + size/2); // last size / 2 elements / les derniers size / 2 elements

    // Merge two sorted half arrays
    // Fusionner deux moities du tableau deja tries
    // TODO / A FAIRE ...
#pragma omp taskwait
    merge(a, size, temp);
  }
}

void printUsage(int argc, char **argv)
{
  printf("Usage: %s [size]\n", argv[0]);
  printf("Example: %s 1024\n", argv[0]);
}

int main(int argc, char **argv)
{
  int *a;
  int *temp;
  int size;

  // Read array size and initialize
  // Lire la taille du tableau et l'initialiser
  if (argc < 2) { 
    printUsage(argc, argv);
    return 0;
  }
  size = atoi(argv[1]);
  a = (int *) malloc(size * sizeof(int));
  temp = (int *) malloc(size * sizeof(int));
  srand(0);
#pragma omp parallel default(none) shared(a, size)
  {
#pragma omp for
    for (int i = 0; i < size; ++i) {
      a[i] = size - i;
    }
  }

  // Sort the array a[size] using mergesort in parallel, using temp as a temporary buffer
  // You need to create threads, call mergesort from one thread, then create tasks for recursive calls.
  // Trier le tableau a[size] avec mergesort en parallel , en utilisant temp comme un buffer temporaire
  // Il faut creer les threads, appeler mergesort depuis un seul thread, puis creer des taches pour les appels recursifs
  // TODO / A FAIRE ...
  // We share the variables a, size, and temp among threads since they are constant
  // On partage les variables a, size et temp parmi les threads car elles sont constantes
  auto start = std::chrono::high_resolution_clock::now();
#pragma omp parallel default(none) shared(a, size, temp)
  {
#pragma omp single
    mergesort(a, size, temp);
  }
  std::chrono::duration<double> sortTime = std::chrono::high_resolution_clock::now() - start;
  printf("Sorting took %.4lf seconds.\n", sortTime.count());

  // Verify if the array is sorted
  // Verifier si le tableau est bien trie
  if (isSorted(a, size)) { printf("The array was properly sorted.\n"); }
  else { printf("There was an error when sorting the array.\n"); }

  // Deallocate the arrays
  // Desallouer les tableaux
  free(a);
  free(temp);
  
  return 0;
}
