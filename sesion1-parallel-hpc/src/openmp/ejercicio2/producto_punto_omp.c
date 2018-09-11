#include <stdio.h>
#include <omp.h>

#define SIZE 1024 //Definimos el tamano del problema

main(int argc, char *argv[]) {
	
    //Declaracion de variables
    double Sum = 0.0, a[SIZE], b[SIZE];
	
    int i;
	
    //Asignamos valores a las posiciones de las matrices
    for (i = 0; i < SIZE; i++) {
		a[i] = 0.5*i; 
        b[i] = 2.0*i;
	}
	
    //Calculamos el producto punto
    //Asignamos la region paralela considerando 
      //una reduccion en el calculo final
    #pragma omp parallel for reduction(+:Sum)
        for (i = 0; i < SIZE; i++) {
    		Sum = Sum + a[i]*b[i];
    	}
	
    //Imprimimos el resultado
    printf("Sum = %f\n", Sum);
}


