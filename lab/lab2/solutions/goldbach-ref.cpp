#include <iostream>
#include <chrono>
#include <cmath>
#include "omp.h"

#define N 8192

/**
  * Calculer si x est un nombre premier ou pas
  */
bool estPremier(int x)
{
  if (x < 2) { return false; }
  for (int i = 2; i <= x / 2; i++) {
    if (x % i == 0) { return false; }
  }
  return true;
}

/**
  * Calculer le nombre de paires (i, j) tel que i + j = x et i et j sont des nombres premiers.
  */
int goldbach(int x)
{
  int compte = 0;
  if (x <= 2) { return 0; }
  for (int i = 2; i <= x / 2; i++) {
    if (estPremier(i) && estPremier(x - i)) { compte++; }
  }
  return compte;
}

int main()
{
  int goldbachVrai;
  int numPairs[N];
  for (int i = 2; i < N; i += 2) { numPairs[i] = 0; }

  // La version sequentielle
  auto start = std::chrono::high_resolution_clock::now();
  goldbachVrai = 1;
  for (int i = 2; i < N; i += 2) { 
    numPairs[i] = goldbach(i);
    if (numPairs[i] == 0) { goldbachVrai = 2; }
  }
  if (goldbachVrai) { std::cout << "La conjecture de Goldbach est vrai" << std::endl; }
  else { std::cout << "La conjecture de Goldbach est vrai" << std::endl; }
  std::chrono::duration<double> tempsSeq = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps sequentiel: " << tempsSeq.count() << "s\n";

  // Parallelisation sans preciser l'ordonnancement, ce qui donne un ordonnancement static. Donc les derniers N /
  // numThreads iterations seront executees par le dernier thread. Comme la complexite du calcul goldbach(i) est de
  // O(i^2), cette strategie genere un desequilibrage de charge parmi les threads.
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  std::chrono::duration<double> temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps ordonnancement par defaut: " << temps.count() << "s\n";

  // Dans cette strategie, l'ordonnancement static distribue les blocs de taille 32 de maniere circulaire parmi les
  // threads, donc la charge elevee des dernieres iterations est mieux equilibree parmis les threads. Neanmoins, il y
  // aura un desequilibrage car les dernieres 32 iterations~(les plus couteuses) seront toujours executees par un seul
  // thread.
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(static, 32)
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps schedule(static, 32): " << temps.count() << "s\n";

  // Dans cette strategie, l'ordonnancement dynamic distribue les iterations de maniere ``premier arrive premier
  // servi''. La taille de bloc est 1 par defaut donc les dernieres iterations sont bien equilibrees
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic)
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps schedule(dynamic): " << temps.count() << "s\n";

  // Pareil que le dernier, sauf la taille de bloc est 32
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(dynamic, 32)
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps schedule(dynamic, 32): " << temps.count() << "s\n";

  // Dans cette strategie, l'ordonnancement guided commence par distribuer de blocs de suffisament grande taille, puis
  // au fil des iterations il diminue la taille de bloc (jusqu'a 1 par defaut).
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(guided)
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps schedule(guided): " << temps.count() << "s\n";

  // Pareil que le dernier sauf la taille de bloc diminue jusqu'a 32
  start = std::chrono::high_resolution_clock::now();
#pragma omp parallel for schedule(guided, 32)
  for (int i = 2; i < N; i += 2) {
    numPairs[i] = goldbach(i);
  }
  temps = std::chrono::high_resolution_clock::now() - start;
  std::cout << "Temps schedule(guided, 32): " << temps.count() << "s\n";

  return 0;
}
