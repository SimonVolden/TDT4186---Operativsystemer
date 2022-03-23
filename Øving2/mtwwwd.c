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
#include "bbuffer.h"
#include "sem.h"

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

char root[256];

/* Makes the consumers load work from bbuffer and handle the request from clients */
void *handle_request(void *arg)
{
    char path[256];
    char *fbuffer = NULL;
    FILE *fp;

    while (1)
    {
        /* get file descriptor from bbuffer */
        int fd;
        fd = bb_get((BNDBUF *)arg);
        if (fd)
        {

            /* clears request buffer */
            memset(buffer, 0, sizeof(buffer));
            int n = read(fd, buffer, sizeof(buffer) - 1);
            if (n < 0)
                error("ERROR reading from socket");

            /* get path */
            char buffercpy[MAXREQ];
            strcpy(buffercpy, buffer);
            char *token;
            token = strtok(buffercpy, " ");
            printf("[Worker Thread %li] ", (long)pthread_self());
            printf("Request type: %s; ", token);
            token = strtok(NULL, " ");

            printf("Request path: %s\n", token);
            strcpy(path, root);

            /* check if default route */
            if (token[1] == 0)
                strcpy(token, "/index.html");

            strcat(path, token);
            printf("full path: %s\n", path);

            /* opens requested file, and writes content into fbuffer */
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
            /* sends page if found */
            if (fbuffer != NULL)
            {
                strcpy(body, fbuffer);

                snprintf(msg, sizeof(msg),
                         "HTTP/1.0 200 OK\n"
                         "Content-Type: text/html\n"
                         "Content-Length: %ld\n\n%s",
                         strlen(body), body);
            }
            else // sends 404 if page not found
            {
                snprintf(body, sizeof(body),
                         "<html>\n<body>\n"
                         "<h1> 404 Not Found</h1>\n"
                         "The requested URL was not found.\n"
                         "</body>\n</html>");

                snprintf(msg, sizeof(msg),
                         "HTTP/1.0 404 Not Found\n"
                         "Content-Type: text/html\n"
                         "Content-Length: %ld\n\n%s",
                         strlen(body), body);
            }
            n = write(fd, msg, strlen(msg));
            if (n < 0)
                error("ERROR writing to socket");
            /* closes socket */
            close(fd);
        }
    }
}

int main(int argc, char *argv[])
{

    /* Root from args */
    strcpy(root, argv[2]);
    printf("Root: %s\n", root);

    /* Port from args */
    char *a = argv[1];
    int PORT = atoi(a);

    /* Threads from args */
    char *b = argv[3];
    int THREADS = atoi(b);

    /* Bbuffer slots from args */
    char *c = argv[4];
    int BNDBUFSIZE = atoi(c);
    struct BNDBUF *bbuffer = bb_init(BNDBUFSIZE);

    printf("Port: %d\n", PORT);
    printf("Threads: %d\n", THREADS);
    printf("Buffer size: %d\n", BNDBUFSIZE);

    int sockfd, newsockfd;
    socklen_t clilen;
    struct addrinfo *serverinfo;
    struct sockaddr_in6 serv_addr, cli_addr;
    pthread_t threadID[THREADS];

    /* main thread socket, accepts IPv4 and IPv6 */
    sockfd = socket(AF_INET6, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin6_family = AF_INET6;
    serv_addr.sin6_addr = in6addr_any;
    serv_addr.sin6_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr *)&serv_addr,
             sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }
    listen(sockfd, 5);

    /* Create worker threads */
    printf("creating threads\n");
    for (int i = 0; i < THREADS; i++)
    {
        pthread_create(&threadID[i], NULL, handle_request, bbuffer);
    }

    /*Main accept loop, adds file descriptors to bbuffer */
    while (1)
    {
        printf("Ready to take requests on port: %d\n", PORT);
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, NULL, NULL);
        if (newsockfd < 0)
        {
            error("ERROR on accept");
            exit(EXIT_FAILURE);
        }
        bb_add(bbuffer, newsockfd);
    }
}
