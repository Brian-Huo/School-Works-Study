#include <stdio.h>
#include <mpi.h>

int main( int argc, char *argv[] )
{
    int rank, size, rc;
    int packsize, position;
    int value_a;
    double value_b;
    char buffer[100];

    setvbuf( stdout, NULL, _IONBF, 0);
    // Initialize the communicator MPI_COMM_WORLD and import commandline argument
    rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS){  
        printf ("Error starting MPI program. Terminating.\n");      
        MPI_Abort(MPI_COMM_WORLD, rc);   
    }
    // Getting number of processors (size of communicator)   
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    do {
        // Let process 0 boardcast the value to other processes
	if (rank == 0) {
            scanf( "%d %lf", &value_a, &value_b );
            // Pack the input value in a mpi packet
            packsize = 0;
            MPI_Pack( &value_a, 1, MPI_INT, buffer, 100, &packsize, 
    	              MPI_COMM_WORLD );
	    MPI_Pack( &value_b, 1, MPI_DOUBLE, buffer, 100, &packsize, 
    	              MPI_COMM_WORLD );
	}
        MPI_Bcast( &packsize, 1, MPI_INT, 0, MPI_COMM_WORLD );
        MPI_Bcast( buffer, packsize, MPI_PACKED, 0, MPI_COMM_WORLD );
        if (rank != 0) {
            // Unpack the packet and retrieve the value for output
            position = 0;
            MPI_Unpack( buffer, packsize, &position, &value_a, 1, MPI_INT, MPI_COMM_WORLD );
	    MPI_Unpack( buffer, packsize, &position, &value_b, 1, MPI_DOUBLE, MPI_COMM_WORLD );
	}
	
	printf( "Process %d out of %d, received value is %d and %lf\n", rank, size, value_a, value_b );
    } while (value_a >= 0);

    MPI_Finalize( );
    return 0;
}
