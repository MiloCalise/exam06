#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <strings.h>
#include <stdio.h>

typedef struct s_client
{
    int     fd;
    int     id;
    char    *in;
    char    *out;
}   t_client;

typedef struct s_server
{
    int         clients;
    int         next_id;
    t_client    client_tab[128];
}   t_server;

int main(int argc, char **argv)
{
    if (argc != 2)
        return (write(2, "Wrong number of arguments\n", 27), 1);
    int port = atoi(argv[1]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
        return (write(2, "Fatal error\n", 13), 1);
    struct sockaddr_in  addr;
    bzero(&addr, sizeof(addr));
    addr.sin_port = ((port & 0xFF) << 8 | port >> 8);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = 0x0100007F;
    if (bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        return (write(2, "Fatal error\n", 13), 1);
    if (listen(sockfd, 128) == -1)
        return (write(2, "Fatal error\n", 13), 1);
    t_server *serv = calloc(sizeof(t_server), 1);
    if (!serv)
        return (write(2, "Fatal error\n", 13), 1);
    while (1)
    {
        fd_set  rset;
        fd_set  wset;
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_SET(sockfd, &rset);
        int maxfd = sockfd;
        for (int i = 0; i < serv->clients; i++)
        {
            FD_SET(serv->client_tab[i].fd, &rset);
            if (serv->client_tab[i].out)
                FD_SET(serv->client_tab[i].fd, &wset);
            if (serv->client_tab[i].fd > maxfd)
                maxfd = serv->client_tab[i].fd;
        }
        if (select(maxfd + 1, &rset, &wset, NULL, NULL) == -1)
            return (write(2, "Fatal error\n", 13), 1);
        for (int i = 0; i < serv->clients; i++)
        {
            if (FD_ISSET(serv->client_tab[i].fd, &wset))
        }
    }
    close (sockfd);
    return (0);
}
