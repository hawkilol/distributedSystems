#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int num_sockets = 10;
    int sockfd[num_sockets];
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    int execution;

    if (argc < 2)
    {
        fprintf(stderr, "Erro, nenhuma porta indicada\n");
        exit(1);
    }

    FILE *file;
    execution = atoi(argv[1]);
    printf("Execution:%d\n", execution);


    if(execution == 0){
      file = fopen("portsClose.csv", "w");
      fprintf(file, "Port Number,Execution\n");
    }
    else{
      file = fopen("portsClose.csv", "a");
    }

    for (int i = 0; i < num_sockets; i++) {
        printf("socket%d\n", i);

        sockfd[i] = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd[i] < 0)
        {
            error("Erro ao abrir o socket");
        }

        // Força o OS a atribuir uma porta 'aleatoria' a partir de um socket temporario com bind na porta 0
        struct sockaddr_in temp_addr;
        bzero((char *)&temp_addr, sizeof(temp_addr));
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_addr.s_addr = INADDR_ANY;
        temp_addr.sin_port = 0;

        if (bind(sockfd[i], (struct sockaddr *)&temp_addr, sizeof(temp_addr)) < 0)
        {
            error("Erro no binding do socket temporário");
        }

        // retornar a real porta que o OS atribuiu
        struct sockaddr_in actual_addr;
        socklen_t len = sizeof(actual_addr);
        if (getsockname(sockfd[i], (struct sockaddr *)&actual_addr, &len) == 0 &&
            actual_addr.sin_family == AF_INET && len == sizeof(actual_addr))
        {
            printf("Server is listening on port: %d\n", ntohs(actual_addr.sin_port));

            fprintf(file, "%d,%d\n", ntohs(actual_addr.sin_port), execution);

            printf("Closing Socket: %d", i);

            close(sockfd[i]);


        }
        else
        {
            perror("Error getting local port number");
        }

        listen(sockfd[i], 5);
    }
    fclose(file);

    for (int i = 0; i < num_sockets; i++) {
        while (1)
        {
            clilen = sizeof(cli_addr);
            int newsockfd = accept(sockfd[i], (struct sockaddr *)&cli_addr, &clilen);
            if (newsockfd < 0)
            {
                error("Erro ao aceitar");
            }
            bzero(buffer, 256);
            n = read(newsockfd, buffer, 255);
            if (n < 0)
            {
                error("Erro ao ler o socket");
            }
            printf("Mensagem recebida através do socket:\n %s\n", buffer);


            n = write(newsockfd, "Mensagem recebida :)", 19);
            if (n < 0)
            {
                error("Erro ao escrever no socket");
            }
            close(newsockfd);
        }

        close(sockfd[i]);
    }

    return 0;
}