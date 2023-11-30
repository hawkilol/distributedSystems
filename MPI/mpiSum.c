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

    // block size é o tamanho dinamico das partições (dinamico pois varia de acordo com size de processos -np inicializados)
    // da matriz atribuidas a cada processo
    // lembrando que o tamnho do bloco real que vai ser passado na messagem vai ser multiplicado pela ordem mais uma vez

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
    // Cada processo calcula uma parte da matriz C. 
    // Isso reflete a estratégia do problema original,
    // onde partes das matrizes A e B são somadas de forma paralela (sem condição de corrida)
    // para construir a matriz C.
    // Cada processo (rank) vai ser responsavel por uma faixa de linhas da matrix igual ao tamanho do bloco
    // que vai do inicio do bloco até o final dele dinamicamente em relação ao rank atual
    // atribuindo dinamente a responsabilidade por uma partição ao rank
    for (int i = rank * block_size; i < (rank + 1) * block_size; i++)
    {
        for (int j = 0; j < order; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    // O processo de rank 0 recebe as contribuições de outros processos e as incorpora à matriz final C.
    // Isso simula a necessidade de coletar 
    // os resultados parciais de diferentes blocos de matrizes C calculados por diferentes processos.
    // Assim o processo de rank 0 apenas recebe os blocos e os outros mandam os blocos
    // O Send e o Recv recebem como parametro principalmente o endereço do bloco partido
    // e o tamanho do bloco real 
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


    // Depois que o rank 0 receber as partições ele vai printar elas
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
