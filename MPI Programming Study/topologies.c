#include <stdio.h>
#include "mpi.h"

int main(int argc, char *argv[])
{
    int rank, value, size;
    const int false = 0;
    int right, left;
    MPI_Comm new_comm;
    MPI_Status status;

    // Initializing the MPI Program
    MPI_Init(&argc, &argv);

    // Getting the overall size of the MPI communicator
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Create a cart by using the initial communicator
    MPI_Cart_create(MPI_COMM_WORLD, 1, &size, &false, 1, &new_comm);
    // Getting the shifted ranks of left and right processes
    MPI_Cart_shift(new_comm, 0, 1, &left, &right);

    // Getting new size and self-rank from the new communicator
    MPI_Comm_rank(new_comm, &rank);
    MPI_Comm_size(new_comm, &size);

    do {
        if (rank == 0) {
            scanf("%d", &value);       // The first process is getting the value from terminal
            MPI_Send(&value, 1, MPI_INT, right, 0, new_comm);         // Sending value to the right side process
        }
        else {
            // Receiving the value from left side process and sending to the right side process
            MPI_Recv(&value, 1, MPI_INT, left, 0, new_comm, &status);
            MPI_Send(&value, 1, MPI_INT, right, 0, new_comm);
        }
        printf("Process %d of %d, receiving value %d\n", rank, size, value);
        fflush(stdout);
    } while(value >= 0);

    // Terminate the MPI Program
    MPI_Finalize();
    return 0;
}
