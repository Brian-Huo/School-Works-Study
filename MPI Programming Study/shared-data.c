#include <stdio.h> 
#include "mpi.h"  

int main(int argc, char *argv[])  {  
    int size, rank, rc;
    int value;

    setvbuf( stdout, NULL, _IONBF, 0);
    // Initialize the communicator MPI_COMM_WORLD and import commandline argument
    MPI_Init(&argc, &argv);

    // Getting number of processors (size of communicator) 
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size);  

    do{
        // Let process 0 boardcast the value to other processes
        if(rank == 0){
            scanf("%d", &value);
        }
        MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);
        printf("Process %d out of %d, received value is %d\n", rank, size, value); 
        //fflush(stdout);  
    }while(value >= 0);

    MPI_Finalize();
    return 0;
}
