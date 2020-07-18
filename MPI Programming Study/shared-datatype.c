#include <stdio.h> 
#include <mpi.h>  

int main(int argc, char *argv[])  {  
    int rank, rc, size;
    struct { int a; double b;} value;
    MPI_Datatype mydata;
    int blocklens[2] = [1,1];
    MPI_Aint index[2] = [MPI_INT, MPI_DOUBLE];
    MPI_Datatype old_types[2];

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

    // Allocate address for two values in mydata structure
    MPI_Address(&value.a, &index[0]);
    MPI_Address(&value.b, &index[1]);

    // Making the address of mydata relative
    index[0] = 0;
    index[1] = index[1] - index[0];

    // Construct the custom structure and commit it
    MPI_Type_struct(2, blocklens, index, old_types, &mydata);
    MPI_Type_commit(&mydata);

    do{
        // Let process 0 boardcast the value to other processes
        if(rank == 0){
            scanf("%d %lf", &value.a, &value.b);
        }
        MPI_Bcast(&value, 1, mydata, 0, MPI_COMM_WORLD);
        printf("Process %d out of %d, received value is %d and %lf\n", rank, size, value.a, value.b); 
       
    }while(value.a >= 0); 

    // Deallocated and terminal the mpi program
    MPI_Type_free(&mydata);
    MPI_Finalize();
    return 0;
}
