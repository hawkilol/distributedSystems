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

    // block size é o tamanho dinamico das partições (dinamico pois varia de acordo com size de processos -np inicializados)
    // da matriz atribuidas a cada processo
    // lembrando que o tamnho do bloco real que vai ser passado na messagem vai ser multiplicado pela ordem mais uma vez
    // Obs: isso gera problemas quando é usado mais de 4 processos devido a continuidade da memoria
    // (malloc resolveria isso)
    int block_size = ORDER / size;

    int A[ORDER][ORDER], B[ORDER][ORDER], C[ORDER][ORDER];

    // inicializa os valores das matrizes
    for (int i = 0; i < ORDER; i++)
    {
        for (int j = 0; j < ORDER; j++)
        {
            A[i][j] = 1;
            B[i][j] = 1;
            C[i][j] = 0;
        }
    }

    MPI_Status status;

    // O processo 0 vai servir como mestre dos outros e distribuir as matrizes os blocos A e B para eles
    // O Send e o Recv recebem como parametro principalmente o endereço do bloco partido
    // e o tamanho do bloco real
    // o range de linhas dos bloco depende dinamicamente do rank e do tamanho do bloco
    // como nesse exemplo:
    // order = 4
    // slices = 4
    // matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12],[13,14,15,16]]

    // half_order = order // 2

    // for i in range(0, half_order):
    //     for j in range(0, half_order): 
    //         print("1", i, j)

    //     print("\n")
    // for i in range(0, half_order):
    //     for j in range(half_order, 4):  
    //         print("2", i, j)

    //     print("\n")


    // for i in range(half_order, 4):
    //     for j in range(0, half_order): 
    //         print("3", i, j)

    //     print("\n")

    // for i in range(half_order, 4):
    //     for j in range(half_order, 4): 
    //         print("4", i, j)

    //     print("\n")

    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            MPI_Send(&A[i * block_size][0], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(&B[i * block_size][0], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD);
            printf("Sent to rank %d\n", i);
        }
    }
    else
    {
        MPI_Recv(&A[rank * block_size][0], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        MPI_Recv(&B[rank * block_size][0], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
        printf("Receive from source %d, %d\n", status.MPI_SOURCE, status.MPI_SOURCE == 1);
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
        for (int j = 0; j < ORDER; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }

    // O processo de rank 0 recebe as contribuições de outros processos e as incorpora a matriz final C.
    // Isso simula a necessidade de coletar 
    // os resultados parciais de diferentes blocos de matrizes C calculados por diferentes processos.
    if (rank == 0)
    {
        for (int i = 1; i < size; i++)
        {
            MPI_Recv(&C[i * block_size][0], block_size * ORDER, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            printf("Receive from source %d, %d\n", status.MPI_SOURCE, status.MPI_SOURCE == 1);
        }
    }

    // Os outros processos vão mandar o resultado de seus blocos para o 0
    else
    {
        MPI_Send(&C[rank * block_size][0], block_size * ORDER, MPI_INT, 0, 0, MPI_COMM_WORLD);
        printf("Sending to rank %d\n", 0);
    }

    // Depois que o rank 0 receber as partições ele vai printar elas
    if (rank == 0)
    {
        for (int i = 0; i < ORDER; i++)
        {
            for (int j = 0; j < ORDER; j++)
            {
                printf("C[%d][%d] = %d\n", i, j, C[i][j]);
            }
            printf("\n");
        }
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
