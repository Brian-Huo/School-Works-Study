#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[]){
    int rank, size, i, buffer[1];
    MPI_Status status;

    // Initializing the MPI program
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Root process 0 only receiving the message from others
    if (rank == 0) {
        for (i=0; i<100*(size-1); i++) {
            MPI_Recv(buffer, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            printf("Message from process %d with tag %d\n", status.MPI_SOURCE, status.MPI_TAG);
        }
    } else {
		// sending numbers from 0 to 99 to root process
        for (i=0; i<100; i++)
            MPI_Send(buffer, 1, MPI_INT, 0, i, MPI_COMM_WORLD);
    }

    // Terminate the MPI program
    MPI_Finalize();
    return 0;
}
