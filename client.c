#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char nome[256];  // Buffer for nome
    char matricula[256];  // Buffer for matricula

    if (argc < 3)
    {
        fprintf(stderr, "O comando deverá ter o formato: %s host porta\n", argv[0]);
        exit(1);
    }
    portno = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        error("Erro ao abrir o socket");
    }

    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Erro ao encontrar o host\n");
        exit(0);
    }

    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        error("Erro ao conectar");
    }

    printf("Entre com o nome: ");
    bzero(nome, 256);
    fgets(nome, 255, stdin);

    printf("\nEntre com a matricula: ");
    bzero(matricula, 256);
    fgets(matricula, 255, stdin);

    // Write nome to the socket
    n = write(sockfd, nome, strlen(nome));
    if (n < 0)
    {
        error("Erro ao escrever o nome no socket");
    }

    // Write matricula to the socket
    n = write(sockfd, matricula, strlen(matricula));
    if (n < 0)
    {
        error("Erro ao escrever a matricula no socket");
    }
    
     /* Lê da conexão aberta pelo socket da mesma forma que ler de um arquivo */
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        error("Erro ao ler o socket");
    }
    printf("Mensagem recebida através do socket: %s\n", buffer);

    close(sockfd);
    return 0;
}
