/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:39:02 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/07 13:13:01 by abenmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Server.hpp"
#include <time.h>

void err_p(const char *str)
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
    client_socket = accept(server_socket, (struct sockaddr *)&connection_addr, &addr_len);
    return (client_socket);
}

int main()
{
    std::vector<struct pollfd> fds;
    int server_socket = set_server_socket();
    struct pollfd new_fd;
    new_fd.fd = server_socket;
    new_fd.events = POLLIN;
    fds.push_back(new_fd);
    while(1)
    {
        if ((poll(&fds[0], fds.size(), -1)) <= 0)
            err_p("poll");
        for (size_t i = 0; i < fds.size(); i++) {
            
            if (fds[i].revents & POLLIN) {

                if (fds[i].fd == server_socket) {
                    
                    int newfd;
                    newfd = accept_client(server_socket);
                    if (newfd < 0) {
                        perror("accept");
                    } else {
                        std::cout << "hey\n";
                        new_fd.fd = newfd;
                        new_fd.events = POLLIN | POLLOUT;
                        fds.push_back(new_fd);
                    }
                } else {
                    char storage_buff[4096];
                    int bytes_read;
                    bytes_read = recv(fds[i].fd, storage_buff, 4096, 0);
                    int sender_fd = fds[i].fd;
                    if (bytes_read <= 0)
                    {
                        if (bytes_read == 0)
                            std::cout << "Client [" << fds[i].fd << "] End Connection" << std::endl;
                        else
                            perror("recv");
                        close(fds[i].fd);
                        std::vector<struct pollfd>::iterator it = fds.begin();
                        for (int j; j < i; j++)
                        {
                            it++;
                        }
                        std::cout << it << "hey\n";
                        std::advance(it, i);
                        fds.erase(it);
                    }
                    else
                    {
                        for(size_t j = 0; j < fds.size(); j++)
                        {
                            int dest_fd = fds[j].fd;
                            if (dest_fd != server_socket && dest_fd != sender_fd)
                            {
                                int check = send(dest_fd, storage_buff, bytes_read, 0);
                                if (check == -1)
                                {
                                    perror("send");
                                }
                            }
                        }
                    }
                } 
            }
        }
    }
    return 0;
}
