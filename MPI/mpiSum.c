
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>


int main(int argc, char **argv)
{
    int order = 500;
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int block_size = order / size;

    int A[order][order], B[order][order], C[order][order];

    // inicializa os valores das matrizes

    for (int i = 0; i < order; i++)
    {
        for (int j = 0; j < order; j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }
    // Soma a matrix

    for (int i = rank * block_size; i < (rank + 1) * block_size; i++)
    {
        for (int j = 0; j < order; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            MPI_Status status;

            MPI_Recv(&C[i * block_size][i * block_size], block_size * order, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Receive from source %d, %d\n", status.MPI_SOURCE, status.MPI_SOURCE == 1);

        }
    }
    else
    {
        printf("Sending to rank %d\n", rank);
        MPI_Send(&C[rank * block_size][rank * block_size], block_size * order, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Sent to rank %d\n", rank);

    }

    if (rank == 0)
    {
        for (int i = 0; i < order; i++)
        {
            for (int j = 0; j < order; j++)
            {
                printf("C[%d][%d] = %d\n", i, j, C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
