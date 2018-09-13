#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]) {

int nthreads, tid;

/* Crear un grupo de hilos dandoles su propia copia de las variables*/
#pragma omp parallel private(nthreads, tid)
 {

 /* Obtener el numero del hilo */
 tid = omp_get_thread_num();
 printf("Hola mundo desde el hilo = %d\n", tid);

 /* Solo el hilo maestro realizara esta parte */
 if (tid == 0) 
 {
  nthreads = omp_get_num_threads();
  printf("Numero de Hilos = %d\n", nthreads);
 }

 } /* Aqui todos los hilos se unen al hilo maestro*/

}

