/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:39:02 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/11 11:33:40 by abenmous         ###   ########.fr       */
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
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
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
std::string recv_request(int _socket)
{
    char storage_buff[4096];
    int bytes_read;
    bytes_read = recv(_socket, storage_buff, 4096, 0);
    storage_buff[bytes_read] = '\0';
    std::string ret = storage_buff;
    return (ret);
}
void push_fds(int _socket, size_t flags, std::vector<struct pollfd> &fds)
{
    struct pollfd new_fd;
    new_fd.fd = _socket;
    new_fd.events = flags;
    fds.push_back(new_fd);
}

void pull_fds(int i, std::vector<struct pollfd> &fds)
{
    std::cout << "Client [" << fds[i].fd << "] End Connection" << std::endl;
    close(fds[i].fd);
    std::vector<struct pollfd>::iterator it = fds.begin();
    std::advance(it, i);
    fds.erase(it);
}

int main()
{
    std::vector<struct pollfd> fds;
    std::vector<Client> Client_Vec;
    std::string buff;
    int server_socket = set_server_socket();
    push_fds(server_socket, POLLIN, fds);
    while(1)
    {
        if ((poll(&fds[0], fds.size(), -1)) <= 0)
            err_p("poll");
        for (size_t i = 0; i < fds.size(); i++) 
        {
            if (fds[i].revents & POLLIN) 
            {
                if (fds[i].fd == server_socket) 
                {
                    int newfd;
                    newfd = accept_client(server_socket);
                    if (newfd < 0) 
                        perror("accept");
                    else
                        push_fds(newfd, POLLIN | POLLOUT, fds);
                }
                else 
                {
                    buff = recv_request(fds[i].fd);
                    Client Clone(buff);
                    Client_Vec.push_back(Clone);
                    if (buff.length() <= 0)
                        pull_fds(i, fds);
                }
            }
        }
    }
}
