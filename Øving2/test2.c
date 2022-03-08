#include <WinSock2.h>
#include <stdio.h>
#include <sys/stat.h> // source : official account(C language and CPP programming)

int send_(int s, char buf, int len)
{
    int total;
    int bytesleft;
    int n;
    total = 0;
    bytesleft = *len;
    while (total < *len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            break;
        }
        total += n;
        bytesleft -= n;
    }
    *len = total;
    return n == -1 ? -1 : 0;
}

void req(char *request, int client_sock)
{
    char arguments[BUFSIZ];
    strcpy(arguments, “./”);

    char command[BUFSIZ];
    sscanf(request, “% s % s”, command, arguments + 2);

    char *extension = “text / html”;
    char *content_type = “text / plain”;
    char *body_length = “Content - Length : “;

    FILE *rfile = fopen(arguments, “rb”);

    char *head = “HTTP / 1.1 200 OKrn”;
    int len;
    char ctype[30] = “Content - type : text / htmlrn”;
    len = strlen(head);

    send_(client_sock, head, &len);
    len = strlen(ctype);
    send_(client_sock, ctype, &len);

    struct stat statbuf;

    char length_buf[20];
    fstat(fileno(rfile), &statbuf);
    itoa(statbuf.st_size, length_buf, 10);
    send(client_sock, body_length, strlen(body_length), 0);
    send(client_sock, length_buf, strlen(length_buf), 0);

    send(client_sock, “n”, 1, 0);
    send(client_sock, “rn”, 2, 0);

    char read_buf[1024];
    len = fread(read_buf, 1, statbuf.st_size, rfile);
    if (send_(client_sock, read_buf, &len) == -1)
    {
        printf(“error !”);
    }

    return;
}

int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0)
    {
        exit(1);
    }

    int skt = socket(PF_INET, SOCK_STREAM, 0);
    if (skt == -1)
    {
        return -1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr(“127.0.0.1”);
    memset(&(server_addr.sin_zero), ‘0’, 8);

    if (bind(skt, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        return -1;
    }

    if (listen(skt, 10) == -1)
    {
        return -1;
    }

    while (1)
    {

        printf(“Listening … …n”);
        struct sockaddr_in c_skt;
        int s_size = sizeof(struct sockaddr_in);
        int access_skt = accept(skt, (struct sockaddr *)&c_skt, &s_size);

        char buf[1024];
        if (recv(access_skt, buf, 1024, 0) == -1)
        {
            exit(1);
        }

        req(buf, access_skt);
    }
}