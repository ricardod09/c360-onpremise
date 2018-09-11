/////////////////////////////////////////////////////////////////////////////
//
// Handytec
//
// Ejemplo:		Wave Equation
//
// Problem:		dphi/dt = v dphi/dx
//
// Metodo:		Finite Difference Method with second order central differences
//				and Fourth Order Runge-Kutta Method, parallelized with MPI
//
// Compilacion:	g++ wave_equation.cpp -o wave_equation -lmpi -DUSE_BLOCKING_COMMUNICATION	OR
//				mpicxx wave_equation.cpp -o wave_equation -DUSE_BLOCKING_COMMUNICATION
//
// Ejecucion:	mpirun  -n 4 wave_equation								OR
//				srun -n 4 --nodes 1 --ntasks-per-node 4 wave_equation
//
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <math.h>
#include <mpi.h>
#include <string.h>

using namespace std;

// Global variables
const double	x_min		=  0.000;
const double	x_max		= 10.000;
const double	t_min		=  0.000;
const double	t_max		= 10.000;
const double	Delta_x		=  0.05;
const double	Delta_t		=  0.02;
const int		N_x			=  (x_max-x_min)/Delta_x+1;
const int		N_t			=  (t_max-t_min)/Delta_t+1;
const double	v			=  1.00;

// Function declarations
void	f(double* k, double* phi, int myN_x, int myID, int N_Procs);
void	write(MPI_File& file, double* phi, int l, int myN_x, int prevN_x, int N_x, int myID, int N_Procs);
void	exchange(double* phi, int myN_x, int myID, int N_Procs);

int		main(int argc, char** argv)
{
	// Simulation parameters
	double		x			= 0;
	double		t			= 0;
	int			i			= 0;
	int			l			= 0;
	int			myID		= 0;
	int			myN_x		= 0;
	int			prevN_x		= 0;
	int			N_Procs     = 0;
	double		wtime		= 0.0;	// Record the starting time
	MPI_File	file;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &N_Procs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myID);
	
	if(myID==0)
	{
		wtime	= MPI_Wtime();
	}
	
	// Compute my number of grid points
	if(N_x%N_Procs)
	{
		myN_x = N_x/N_Procs + 1;
		if(myID==N_Procs-1)
		{
			myN_x	= N_x - myN_x*(N_Procs-1);
		}
	}
	else
	{
		myN_x = N_x/N_Procs;
	}
	
	// Allocate arrays
	double*		tempPhi		= new double [myN_x+2];	// NOTE: The extra 2 array elements being allocated here are to store the ghost points
	double*		k1			= new double [myN_x+2];
	double*		k2			= new double [myN_x+2];
	double*		k3			= new double [myN_x+2];
	double*		k4			= new double [myN_x+2];
	double*		phi			= new double [myN_x+2];
	
	memset(k1, 0.0, (myN_x+2)*sizeof(double));
	memset(k2, 0.0, (myN_x+2)*sizeof(double));
	memset(k3, 0.0, (myN_x+2)*sizeof(double));
	memset(k4, 0.0, (myN_x+2)*sizeof(double));
		
	// Set initial condition
	t			= t_min;
	MPI_Exscan(&myN_x, &prevN_x, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	for(i=1; i<myN_x+1; i++)
	{
		x		= x_min + Delta_x*(prevN_x + i - 1);
		phi[i]	= exp(-5.0*pow(x-3.0, 2))+1.0;
	}
	
	// Open the output file
	MPI_File_open(MPI_COMM_WORLD, "wave_equation.data", MPI_MODE_CREATE|MPI_MODE_WRONLY, MPI_INFO_NULL, &file);
	
	write(file, phi, 0, myN_x, prevN_x, N_x, myID, N_Procs);
	
	// Time marching loop
	for(l=0; l<N_t-1; l++)
	{
		t	+= Delta_t;
		
		exchange(phi, myN_x, myID, N_Procs);
		f(k1, phi, myN_x, myID, N_Procs);
		
		for(i=1; i<myN_x+1; i++)
		{
			tempPhi[i]	= phi[i] + Delta_t/2*k1[i];
		}
		exchange(tempPhi, myN_x, myID, N_Procs);
        f(k2, tempPhi, myN_x, myID, N_Procs);
		
		for(i=1; i<myN_x+1; i++)
		{
			tempPhi[i]	= phi[i] + Delta_t/2*k2[i];
		}
		exchange(tempPhi, myN_x, myID, N_Procs);
		f(k3, tempPhi, myN_x, myID, N_Procs);
		
		for(i=1; i<myN_x+1; i++)
		{
			tempPhi[i]	= phi[i] + Delta_t  *k3[i];
		}
		exchange(tempPhi, myN_x, myID, N_Procs);
		f(k4, tempPhi, myN_x, myID, N_Procs);
		
		for(i=1; i<myN_x+1; i++)
		{
			phi[i]	=  phi[i] + Delta_t*(k1[i]/6 + k2[i]/3 + k3[i]/3 + k4[i]/6);
		}
		
		// Write the solution
		write(file, phi, l, myN_x, prevN_x, N_x, myID, N_Procs);
		
		if(myID==0)
		{
			cout << "t = " << t << endl;
		}
	}
		
	// Close the output file
	MPI_File_close(&file);
	
	// Deallocate arrays
	delete [] tempPhi;
	delete [] k1;
	delete [] k2;
	delete [] k3;
	delete [] k4;
	delete [] phi;
	
	if(myID==0)
	{
		wtime	= MPI_Wtime() - wtime;	// Record the end time and calculate elapsed time
		cout << "Simulation took " << wtime/N_t << " seconds per time step with " << N_Procs << " processes" << endl;
	}
	
	MPI_Finalize();
	
	return 0;
}

void	f(double* k, double* phi, int myN_x, int myID, int N_Procs)
{	
	if		(myID==0)
	{
		k[1] = 0;
		for(int i=2; i<myN_x+1; i++)
		{
			k[i]	= -v/(2*Delta_x)*(phi[i+1]  -phi[i-1]);
		}
	}
	else if	(myID==N_Procs-1)
	{
		for(int i=1; i<myN_x; i++)
		{
			k[i]	= -v/(2*Delta_x)*(phi[i+1]  -phi[i-1]);
		}
		k[myN_x]	= -v/(  Delta_x)*(phi[myN_x]-phi[myN_x-1]);
	}
	else
	{
		for(int i=1; i<myN_x+1; i++)
		{
			k[i]	= -v/(2*Delta_x)*(phi[i+1]  -phi[i-1]);
		}
	}
	
	return;
}

void	write(MPI_File& file, double* phi, int l, int myN_x, int prevN_x, int N_x, int myID, int N_Procs)
{
	int			N_CharPer_x		= 7;								// NOTE: we are representing the value at phi[i] in no more than 7 characters
	int			N_BytesPer_x	= N_CharPer_x * sizeof(char);		
	int			N_BytesPer_l	= N_x * N_BytesPer_x;
	int			Offset			= prevN_x * N_BytesPer_x;	
	char		buffer[myN_x*N_BytesPer_x];
	
	// Note: This function is taking care of writing out phi from each processor at every timestep. Two issues that need to be overcome
	// are that firstly we want each processor to be able to write its portion of data to the one output file, and secondly
	// we want to output ascii data (as opposed to binary data) so that we can read the output file in a text editor
	// (or import it into Matlab). The parallel writing of a file is accomplished with the MPI File IO functions, where all 
	// we need to do is tell each process where in the file to start dumping its data. Converting the data to ascii requires
	// allocating a buffer to store 'char's and converting each double precision number to ascii. To elaborate on this point
	// via an example, the double precision number 3.141592652589793238462643 requires 8 bytes of memory, where as storing
	// this number as an array of char's would require 27. This is because each char requires 1 byte, so we need 1 byte
	// for every digit, plus the decimal point, and the 'null' character \0 which goes at the end and is required by every string.
	// This is why writing binary data can be more efficient in terms of disk space and speed of reading or writing. The 
	// disadvantage however is that binary formats can vary between platforms and it is not as easy to inspect the data.
	
	// Convert the array of floats into a string
	for(int i=1; i<myN_x+1; i++)
	{
		if(i==myN_x && myID==N_Procs-1)
		{
			sprintf(buffer+(i-1)*N_CharPer_x, "%+.3f\n", phi[i]);
		}
		else
		{
			sprintf(buffer+(i-1)*N_CharPer_x, "%+.3f\t", phi[i]);
		}
	}
	
	MPI_File_seek(file, l*N_BytesPer_l + Offset, MPI_SEEK_SET);
	MPI_File_write(file, buffer, myN_x * N_CharPer_x, MPI_CHAR, MPI_STATUS_IGNORE);
	
	return;
}

void	exchange(double* phi, int myN_x, int myID, int N_Procs)
{	
#ifdef USE_BLOCKING_COMMUNICATION		// Use 'blocking' sends and recieves
	
	MPI_Status	status;
	
	if(myID>0)				
	{
		MPI_Send(&phi[1],		1,	MPI_DOUBLE,	myID-1,	1, MPI_COMM_WORLD);				// Send    phi[l][1]   to   myID-1
	}
	if(myID<N_Procs-1)	
	{
		MPI_Recv(&phi[myN_x+1],	1,	MPI_DOUBLE,	myID+1,	1, MPI_COMM_WORLD, &status);	// Receive phi[l][N+1] from myID+1
		MPI_Send(&phi[myN_x],	1,	MPI_DOUBLE,	myID+1,	2, MPI_COMM_WORLD);				// Send    phi[l][N]   to   myID+1
	}
	if(myID>0)				
	{
		MPI_Recv(&phi[0],		1,	MPI_DOUBLE,	myID-1,	2, MPI_COMM_WORLD, &status);	// Receive phi[l][0]   from myID-1
	}
	
#else									// Use 'non-blocking' sends and recieves
	
	MPI_Status	statuses[4];
	MPI_Request	requests[4];
	int			commCount	= 0;
	
	if(myID > 0)				
	{
		MPI_Isend(&phi[1],		1,	MPI_DOUBLE,	myID-1,	1, MPI_COMM_WORLD, &requests[commCount++]);	// Send     phi[l][1]   to   myID-1
		MPI_Irecv(&phi[0],		1,	MPI_DOUBLE,	myID-1,	2, MPI_COMM_WORLD, &requests[commCount++]);	// Receive  phi[l][0]   from myID-1
	}
	if(myID < N_Procs-1)	
	{
		MPI_Irecv(&phi[myN_x+1],1,	MPI_DOUBLE,	myID+1,	1, MPI_COMM_WORLD, &requests[commCount++]);	// Receive  phi[l][N+1] from myID+1
		MPI_Isend(&phi[myN_x],	1,	MPI_DOUBLE,	myID+1,	2, MPI_COMM_WORLD, &requests[commCount++]);	// Send     phi[l][N]   to   myID+1
	}
	
	// NOTE: Here we are using 'non-blocking' sends and recieves. the difference compared to the corresponding blocking sends and recieves
	// is the request that is the last argument to each function. Following the communication there is a wait all function which
	// causes the program to halt until all the data has been sent and received. There is a separate request variable for each call 
	// and these are stored in an array. It should be apparent that the processors holding the end parts of the grid will only have
	// 2 requests (one send and one receive), compared to the inner part which have 4 requests (2 sends and 2 receives). The commCount
	// variable keeps track of how many request a given processor has and when a call is made the corresponding request is passed into 
	// the function and the count is incremented. For example at the start of the communcation section commCount is zero. For process 1
	// (say) it will try to send to process 0 (in the first if statement), so requests[0] is passed into the send function call and 
	// commCount is incremented to 1 after that. Then when process 1 enters the next if statement and tries to receive from process 2,
	// it will pass requests[1] and commCount will be incremented to 2. The reason for this is so that in the wait all function call, 
	// each process knows how many calls it needs to wait for. It should also be noted that the idea of having the commCount variable
	// is not a fundamental feature of non-blocking communication, but is just one way of doing things. The important thing however is
	// that each process be waiting for the correct number of sends and receives. */}
	
	MPI_Waitall(commCount, requests, statuses);
	
#endif
	return;
}
