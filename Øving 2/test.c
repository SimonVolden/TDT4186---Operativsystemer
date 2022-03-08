#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>

int main(int argc, char **argv)
{
    struct addrinfo hints, *server;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE || SOCK_NONBLOCK;
    getaddrinfo(NULL, "80", &hints, &server);

    int sockfd = socket(server->ai_family,
                        server->ai_socktype, server->ai_protocol);
    bind(sockfd, server->ai_addr, server->ai_addrlen);
    listen(sockfd, 10);

    struct sockaddr_storage client_addr;
    socklen_t addr_size = sizeof client_addr;
    char headers[] = "HTTP/1.0 200 OK\r\n Server : CPi\r\nContent - type : text / html\r\n\r\n ";
    char buffer[2048];
    char html[] = "<html><head><title>Temperature</title></head> <body> {\"humidity\":81%,\"airtemperature\":23.5C}</p></body></html>\r\n";
    char data[2048] = {0};
    snprintf(data, sizeof data,
             "%s %s", headers, html);

    for (;;)
    {
        int client_fd = accept(sockfd,
                               (struct sockaddr *)&client_addr, &addr_size);
        if (client_fd > 0)
        {
            int n = read(client_fd, buffer, 2048);
            printf("%s", buffer);
            fflush(stdout);
            n = write(client_fd, data, strlen(data));
            close(client_fd);
        }
    }
    return (EXIT_SUCCESS);
}