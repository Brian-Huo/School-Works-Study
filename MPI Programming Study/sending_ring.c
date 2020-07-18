#include <stdio.h>
#include "mpi.h"

int main(int  argc, char *argv[])
{
    int rank, value, size;
    MPI_Status status;

    // Initializing the MPI Program
    MPI_Init(&argc,&argv);

    // Getting the rank and size from MPI Program
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Sending value to the next rank and receiving from the previous rank
    do {
        // Rank 0 doesn't need to receive from other processes
        if (rank == 0) {
            scanf("%d", &value);          // Rechieving the value from terminal
            MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        else {
            // Receiving the value from the previous rank
            MPI_Recv(&value, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &status);
            // If the current rank has next rank, sending the value to it
            if (rank < size - 1)
                MPI_Send(&value, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD);
        }
        printf("Process %d of %d, receiving value %d\n", rank, size, value);
        fflush(stdout);
    } while(value >= 0);

    MPI_Finalize();
    return 0;
}