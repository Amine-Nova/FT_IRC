/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:13:34 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/07 12:40:32 by abenmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include<iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <vector>
#include <poll.h>
#include <string>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#define CONNECTION_PORT 3500

class Client
{
    public:
        Client();
        ~Client();
};

Client::Client()
{
}

Client::~Client()
{
}


/*void err_p(const char *str)
{
    perror(str);
    exit(1);
}

int set_server_socket()
{
    int server_socket, value = 1;
    struct sockaddr_in server_addr;
    
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        err_p("Socket creation failed");
    if ((setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value))) < 0)
        err_p("Couldn't set options");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(CONNECTION_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    memset(server_addr.sin_zero, 0, 8);
    if ((bind(server_socket, (struct sockaddr *)&server_addr, sizeof(struct sockaddr))) < 0 \
        || (listen(server_socket, 10)) < 0)
        err_p("Couldn't listen for connections");
    return (server_socket);
}

int accept_client(int server_socket)
{
    int client_socket;
    socklen_t addr_len;
    struct sockaddr_in connection_addr;
    addr_len = sizeof(connection_addr);
    if ((client_socket = accept(server_socket, (struct sockaddr *)&connection_addr, &addr_len)) < 0)
        err_p("Couldn't establish connection with client");
    return (client_socket);
}

void receive_request(int _socket)
{
    char storage_buff[4096];
    int bytes_read;
    int msize = 0;
    while((bytes_read = recv(_socket, storage_buff, 4096, 0)))
    {
        storage_buff[bytes_read] = '\0';
        msize += bytes_read;
        if (msize > 4095 || storage_buff[msize - 1] == '\n')
            break ;
    }
    if (bytes_read == 0)
        std::cout << "Connection Closed" << std::endl;
    else if(bytes_read == 0)
    {
        perror("err");
        exit(1);
    }
    std::cout << "Message from client [" << _socket << "] : " << storage_buff << std::endl;
}

int main()
{
    std::vector<Client> Client;
    int client_socket;
    char message[7]="Hello\n";
    int server_socket = set_server_socket();
    fd_set read_fds, readfds_clone, write_fds, writefds_clone;
    int socket_num = server_socket;
    FD_ZERO(&read_fds);
    FD_ZERO(&write_fds);
    FD_SET(server_socket, &read_fds);
    while(1)
    {
        // timeval timeout;
        // timeout.tv_sec = 1;
        // timeout.tv_usec = 0;
        readfds_clone = read_fds;
        writefds_clone = write_fds;
        if (select(socket_num + 1, &readfds_clone, &writefds_clone, NULL, NULL) < 0)
            err_p("select");
        for(int i = 3; i <= socket_num; i++)
        {
            if (i == server_socket && FD_ISSET(i, &readfds_clone))
            {
                client_socket = accept_client(server_socket);
                FD_SET(client_socket, &read_fds);
                if (client_socket > socket_num)
                    socket_num = client_socket;
            }
            else if (FD_ISSET(i, &readfds_clone))
            {
                receive_request(i);
                FD_SET(i, &write_fds);
            }
            else if (FD_ISSET(i, &writefds_clone))
            {
                send(i, message, strlen(message), 0);
                FD_CLR(i, &write_fds);
            }
        }
    }
    close(client_socket);
    close(server_socket);
}
*/