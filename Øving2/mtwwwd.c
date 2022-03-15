#define MAXREQ (4096 * 1024)
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>

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
    while (counter < 50)
    {
        counter += 1;
        printf("Ready to take requests on port: %d", PORT);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
        {
            error("ERROR on accept");
            exit(EXIT_FAILURE); // Kanskje fjern
        }
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
        strcat(path, token);
        printf("full path: %s\n", path);

        if ((fp = fopen(path, "r")) == NULL)
        {
            printf("error\n");
            // add 404 error here
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
        if (fbuffer != NULL)
        {
            strcpy(body, fbuffer);
        }
        else
        {
            snprintf(body, sizeof(body),
                     "<html>\n<body>\n"
                     "<h1>Hello web browser</h1>\nYour request was\n"
                     "<pre>%s</pre>\n"
                     "</body>\n</html>",
                     buffer);
        }
        snprintf(msg, sizeof(msg),
                 "HTTP/1.1 200 OK\n"
                 "Content-Type: text/html\n"
                 "Content-Length: %d\n\n%s",
                 strlen(body), body);

        n = write(newsockfd, msg, strlen(msg)); // Det som faktisk sendes, clienten leser på denne socketen
        if (n < 0)
            error("ERROR writing to socket");
        close(newsockfd);
        printf("ENDS");
    }
}