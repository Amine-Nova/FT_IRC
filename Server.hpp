/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:13:34 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/15 17:00:38 by abenmous         ###   ########.fr       */
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
#include <sstream>
#define CONNECTION_PORT 3500

class Client
{
    private:
        int fd;
        std::vector<std::string> Data;
        std::string Pass;
        std::string Nick;
        std::string User;
    public:
        Client();
        Client(std::string Req);
        Client &operator=(const Client &obj);
        void set_attr(std::string Req);
        void user_parse(std::string user);
        void set_data();
        std::string get_user();
        std::string get_nick();
        std::string get_pass();
        int get_fd();
        void set_fd(int fd);
        void set_user(std::string u);
        void set_nick(std::string n);
        ~Client();
};

Client::Client()
{
}

std::string trim_all(std::string all)
{
    size_t lst = all.find_last_not_of('\r');
    all.erase(lst + 1);
    return (all);
}

void Client::set_attr(std::string Req)
{
    std::string user, pass , nick, all;
    Client Ret;
    std::stringstream iss(Req);
    
    while (std::getline(iss, all, '\n'))
    {
        all = trim_all(all);
        Data.push_back(all);
    }
}
void Client::set_data()
{
    std::vector<std::string>::iterator iter = Data.begin();
    for(; iter != Data.end(); iter++)
    {
        if (!(*iter).compare(0, 4, "PASS"))
            this->Pass = (*iter).substr(5, (*iter).length());
        else if (!(*iter).compare(0, 4, "NICK"))
            this->Nick = (*iter).substr(5, (*iter).length());
        else if (!(*iter).compare(0, 4, "USER"))
            user_parse((*iter));
    }
}
void Client::user_parse(std::string user)
{
    std::string ret, garbage;
    std::stringstream ss(user);
    int i = 0;
    while (ss >> garbage)
    {
        if (i == 1)
            ret = garbage;
        i++;   
    }
    this->User = ret;
}
std::string Client::get_user()
{
    return(this->User);
}
std::string Client::get_nick()
{
    return(this->Nick);
}
std::string Client::get_pass()
{
    return(this->Pass);
}
int Client::get_fd()
{
    return(this->fd);
}
void Client::set_fd(int fd)
{
    this->fd = fd;
}
void Client::set_user(std::string u)
{
    this->User = u;
}
void Client::set_nick(std::string n)
{
    this->Nick = n;
}
Client &Client::operator=(const Client &obj)
{
    if(this != &obj)
    {
        this->fd = obj.fd;
        this->Nick = obj.Nick;
        this->Pass = obj.Pass;
        this->User = obj.User;
    }
    return (*this);
}
Client::~Client()
{
}