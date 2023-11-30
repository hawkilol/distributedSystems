#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ORDER 500

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int block_size = ORDER / size;

    int A[ORDER * ORDER], B[ORDER * ORDER], C[ORDER * ORDER];

    for (int i = 0; i < ORDER * ORDER; i++)
    {
        A[i] = 1;
        B[i] = 1;
        C[i] = 0;
    }

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            MPI_Send(&A[i * block_size * ORDER], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B[i * block_size * ORDER], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
    else
    {

        MPI_Recv(&A[rank * block_size * ORDER], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&B[rank * block_size * ORDER], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }


    for (int i = rank * block_size; i < (rank + 1) * block_size; i++)
    {
        for (int j = 0; j < ORDER; j++)
        {
            C[i * ORDER + j] = A[i * ORDER + j] + B[i * ORDER + j];
        }
    }

    if (rank == 0)
    {

        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&C[i * block_size * ORDER], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }


        for (int i = 0; i < ORDER; i++)
        {
            for (int j = 0; j < ORDER; j++)
            {
                printf("C[%d][%d] = %d\n", i, j, C[i * ORDER + j]);
            }
            printf("\n");
        }
    }
    else
    {
        MPI_Send(&C[rank * block_size * ORDER], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if (rank == 0)
    {
        for (int i = 0; i < ORDER; i++)
        {
            for (int j = 0; j < ORDER; j++)
            {
                printf("C[%d][%d] = %d\n", i, j, C[i * ORDER + j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
