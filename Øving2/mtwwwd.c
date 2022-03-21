#define MAXREQ (4096 * 1024)
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

char path[256];
char root[256];
FILE *fp;
char *fbuffer = NULL;

int main(int argc, char *argv[])
{

    strcpy(root, argv[2]); // dette er root som skal settes fra args
    printf("root: %s\n", root);

    int counter = 0;
    char path[100];
    char *a = argv[1];
    int PORT = atoi(a);

    printf("port: %d\n", PORT);

    int sockfd, newsockfd;
    socklen_t clilen;
    struct addrinfo *serverinfo;
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // AF_ITEN6 == IPv6
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    listen(sockfd, 5);
    // thread
    while (1)
    {
        printf("Ready to take requests on port: %d\n", PORT);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
        {
            error("ERROR on accept");
            exit(EXIT_FAILURE); // Kanskje fjern
        }

        // legg til newsockfd i buffer
        // kjør loop på nytt

        memset(buffer, 0, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
            error("ERROR reading from socket");

        /* get path */
        char buffercpy[MAXREQ];
        strcpy(buffercpy, buffer);
        char *token;
        token = strtok(buffercpy, " ");
        printf("Request type: %s\n", token);
        token = strtok(NULL, " ");
        printf("Request path: %s\n", token);
        strcpy(path, root);

        // check if default route
        if (token[1] == 0)
            strcpy(token, "/index.html");

        strcat(path, token);
        printf("full path: %s\n", path);

        if ((fp = fopen(path, "r")) == NULL)
        {
            printf("error: couldn't open file\n");
            fbuffer = NULL;
        }
        else
        {

            size_t len;
            ssize_t bytes_read = getdelim(&fbuffer, &len, '\0', fp);
            if (bytes_read != -1)
            {
                /* Success, now the entire file is in the buffer */
            }
            fclose(fp);
        }
        // sends page if found
        if (fbuffer != NULL)
        {
            strcpy(body, fbuffer);

            snprintf(msg, sizeof(msg),
                     "HTTP/1.0 200 OK\n"
                     "Content-Type: text/html\n"
                     "Content-Length: %d\n\n%s",
                     strlen(body), body);
        }
        else // sends 404 if page not found
        {
            snprintf(body, sizeof(body),
                     "<html>\n<body>\n"
                     "<h1>Not Found</h1>\n"
                     "The requested URL was not found.\n"
                     "</body>\n</html>");

            snprintf(msg, sizeof(msg),
                     "HTTP/1.0 404 Not Found\n"
                     "Content-Type: text/html\n"
                     "Content-Length: %d\n\n%s",
                     strlen(body), body);
        }
        n = write(newsockfd, msg, strlen(msg));
        if (n < 0)
            error("ERROR writing to socket");
        close(newsockfd);
        printf("socket closed\n");
    }
}

/**
 * hver Threads må være en while, Disse lages før while i main
 * Antallet threads skal være satt.
 * De må ha en funksjon som hele tiden kaller bb get.
 * Den blir blokkert dersom bb er tom
 *
 */