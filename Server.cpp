/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:39:02 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/15 17:00:43 by abenmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"Server.hpp"
#include <time.h>

void err_p(const char *str)
{
    perror(str);
    exit(1);
}

int set_server_socket(size_t port)
{
    int server_socket, value = 1;
    struct sockaddr_in server_addr;
    
    if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0)
        err_p("Socket creation failed");
    if ((setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &value, sizeof(value))) < 0)
        err_p("Couldn't set options");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
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
bool find_client(std::vector<Client> &Client_Vec, int fd)
{
    std::vector<Client>::iterator it = Client_Vec.begin();
    for(;it != Client_Vec.end(); it++)
    {
        if (it->get_fd() == fd)
            return (true);
    }
    return (false);
}
int main(int ac, char **av)
{
    if (ac == 3)
    {
        // set argv port and password
        std::vector<struct pollfd> fds;
        std::vector<Client> Client_Vec;
        std::string buff;
        int pass = atoi(av[2]);
        int server_socket = set_server_socket(atoi(av[1]));
        push_fds(server_socket, POLLIN, fds);
        while(1)
        {
            if ((poll(&fds[0], fds.size(), -1)) <= 0)
                err_p("poll");
            for (size_t i = 0; i < fds.size(); i++) 
            {
                if (fds[i].revents & POLLIN) 
                {
                    if (fds[i].revents & (POLLHUP | POLLERR))
                        close(fds[i].fd);
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
                        if (!find_client(Client_Vec, fds[i].fd))
                        {
                            Client Clone;
                            Clone.set_attr(buff);
                            Clone.set_data();
                            if (atoi(Clone.get_pass().c_str()) == pass)
                            {
                                Clone.set_fd(fds[i].fd);
                                Client_Vec.push_back(Clone);
                            }
                            else
                                std::cout << "Wrong Password" << std::endl;
                        }
                        else
                        {
                            std::vector<Client>::iterator it = Client_Vec.begin();
                            for(; it->get_fd() != fds[i].fd; it++){}
                            it->set_attr(buff);
                            it->set_data();
                            // std::cout << "[" << it->get_fd() << "] : " << it->get_nick() << " | " << it->get_user() << " | " << it->get_pass() << std::endl;
                        }
                    }
                }
            }
        }
    }
}
