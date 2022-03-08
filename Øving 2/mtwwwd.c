#define PORT 6789
#define MAXREQ (4096 * 1024)
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netdb.h>
#include <stdlib.h>

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

/**
 * @brief https://stackoverflow.com/questions/42641936/what-does-struct-sockaddr-in-servaddr-clientaddr-mean
 *
 */
/* struct sockaddr_in
{
    short sin_family;        // e.g. AF_INET, AF_INET6
    unsigned short sin_port; // e.g. htons(3490)
    struct in_addr sin_addr; // see struct in_addr, below
    char sin_zero[8];        // zero this if you want to
};

struct in_addr
{
    unsigned long s_addr; // load with inet_pton()
};

*/

int getaddrinfo(const char *node,
                const char *service,
                const struct addrinfo *hints,
                struct addrinfo **res);

int connect(int socket, const struct sockaddr *address, socklen_t address_len);

int main()
{
    int sockfd, newsockfd;
    socklen_t clilen;
    struct addrinfo *serverinfo;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    bzero((char *)&serv_addr, sizeof(serv_addr));
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
    while (1)
    {
        clilen = sizeof(cli_addr);
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr,
                           &clilen);
        if (newsockfd < 0)
        {
            error("ERROR on accept");
            exit(EXIT_FAILURE); // Kanskje fjern
        }
        bzero(buffer, sizeof(buffer));
        n = read(newsockfd, buffer, sizeof(buffer) - 1);
        if (n < 0)
            error("ERROR reading from socket");
        snprintf(body, sizeof(body),
                 "<html>\n<body>\n <h1>Hello web browser</h1>\nYour request was\n <pre>%s</pre>\n </body>\n</html>\n",
                 buffer);
        snprintf(msg, sizeof(msg),
                 "HTTP/1.0 200 OK\n"
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