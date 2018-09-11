#include <iostream>
#include <mpi.h>	// Libreria para utilizar MPI

#define TAG		0

using namespace std;

int	main(int argc, char** argv)
{
	int			myID;
	int			N_Procs;
	int			dummy;
	MPI_Status	status; 
	
	MPI_Init(&argc, &argv);			// Todos los programas MPI empiezan con MPI_Init
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);	// Determinar my ID
	MPI_Comm_size(MPI_COMM_WORLD, &N_Procs);// Determinar cuantos procesos vamos a utilizar
	
	if(myID==0)				// Solo el proceso 0 entrara en esta seccion
	{
		dummy	= 10;
		cout << "Hello world from Process " << myID << " of " << N_Procs << endl;
		for(int n=1; n<N_Procs; n++)    // Enviar mensaje a cada procesador
		{
			MPI_Send(&dummy, 1, MPI_INT, n, TAG, MPI_COMM_WORLD);
		}
	}
	else				        // Los otros procesos esperaran hasta recibir
	{
		// Recibir el mensaje desde el proceso 0
		MPI_Recv(&dummy, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
		cout << "Hello world from Process " << myID << " of " << N_Procs << endl;
	}
	
	MPI_Finalize; 				// los programas MPI finalizan con MPI_Finalize
	return 0;
}
