#include <stdio.h>
#include <omp.h>

#define SIZE 1024*100 //Definimos el tamano del problema

main(int argc, char *argv[]) {
	
    //Declaracion de variables
    double Sum = 0.0, a[SIZE], b[SIZE];	
    int i;
	
    //Asignamos valores a las posiciones de las matrices
    for (i = 0; i < SIZE; i++) {
	a[i] = 0.5*i; 
        b[i] = 2.0*i;
    }
	
    double start_time = omp_get_wtime();
    //Calculamos el producto punto
    #pragma omp parallel for reduction(+:Sum)
    for (i = 0; i < SIZE; i++) {
       Sum = Sum + a[i]*b[i];
    }
	
    //Calculo del tiempo de ejecucion
    double time = omp_get_wtime() - start_time; 
    //Se imprimen los resultados
    
    printf("Numero de Hilos = %d\n", omp_get_max_threads());
    printf("Sum = %f\n", Sum);
    printf("Tiempo = %f\n", time);

}

