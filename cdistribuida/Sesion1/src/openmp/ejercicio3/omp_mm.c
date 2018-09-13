#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NRA 62*100                 /* Numero de Filas de la Matriz A */
#define NCA 15*100                 /* Numero de Columnas de la Matriz A */
#define NCB 7*100                  /* Numero de Columnas de la MAtriz B */

int main (int argc, char *argv[])
{
int     tid, nthreads, i, j, k, chunk;
static double   a[NRA][NCA],       /* Declaracion y asignacion de memoria */
        	b[NCA][NCB],           
        	c[NRA][NCB] = {0};

chunk = 10*100;                    /* Tamano del pedazo a ser procesado por cada procesador */

/*** Region paralela para asignar valores a cada posicion de las matrices ***/
double start_time = omp_get_wtime();
#pragma omp parallel shared(a,b,c,nthreads,chunk) private(tid,i,j,k)
  {
  tid = omp_get_thread_num();
  if (tid == 0)
    {
    nthreads = omp_get_num_threads();
    printf("Starting matrix multiple example with %d threads\n",nthreads);
    printf("Initializing matrices...\n");
    }
  /*** Inicializar matrices ***/
#pragma omp for schedule (static, chunk) 
  for (i=0; i<NRA; i++)
    for (j=0; j<NCA; j++)
      a[i][j]= i+j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NCA; i++)
    for (j=0; j<NCB; j++)
      b[i][j]= i*j;
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    for (j=0; j<NCB; j++)
      c[i][j]= 0;

  /*** Hacer multiplicacion de matrices compartiendo las iteraciones del loop externo ***/
  printf("Thread %d starting matrix multiply...\n",tid);
  #pragma omp for schedule (static, chunk)
  for (i=0; i<NRA; i++)
    {
    //printf("Thread=%d did row=%d\n",tid,i);
    for(j=0; j<NCB; j++)
      for (k=0; k<NCA; k++)
        c[i][j] += a[i][k] * b[k][j];
    }
  }   
  /*** Final de Region paralela ***/
double time = omp_get_wtime() - start_time; //cronometrar ejecucion
printf("******************************************************\n");
printf ("Done.\n");
printf("Tiempo = %f\n", time);

}

