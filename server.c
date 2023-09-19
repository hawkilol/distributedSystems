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
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char response[512];
    if (argc < 2)
    {
        fprintf(stderr, "Erro, nenhuma porta indicada\n");
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Erro ao abrir o socket");
    }
    bzero((char *)&serv_addr, sizeof(serv_addr));
    /* Converte um vetor de caracteres para inteiro */
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; /* Indica o endereço da máquina local */
    serv_addr.sin_port = htons(portno);
    /*
    if( bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
    error("Erro no binding");
    }
    /* Aguarda conexão no socket indicado por sockfd. O segundo argumento indica o número
    de conexões que podem ser manipuladas. */
    listen(sockfd, 5);
    clilen = sizeof(cli_addr);
    /* Tenta aceitar a conexão que chegou. A função accept irá bloquear a execução até que uma conexão
    com o cliente seja estabelecida. */
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
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

    sprintf(response, "Mensagem recebida :) ->(%s)", buffer);

    n = write(newsockfd, response, sizeof(response));
    if (n < 0)
    {
        error("Erro ao escrever no socket");
    }
    close(newsockfd);
    close(sockfd);
    return 0;
}
