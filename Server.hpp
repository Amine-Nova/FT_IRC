/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 16:13:34 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/10 18:31:10 by abenmous         ###   ########.fr       */
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
        std::string Pass;
        std::string Nick;
        std::string User;
    public:
        Client();
        Client(char *request);
        void user_parse(std::string user);
        std::string get_user();
        std::string get_nick();
        std::string get_pass();
        ~Client();
};

Client::Client()
{
}
Client::Client(char *request)
{
    std::string Req = request;
    std::string user, pass , nick, all;
    Client Ret;
    std::stringstream iss(Req);
    while (std::getline(iss, all, '\n'))
    {
        if (!all.compare(0, 4, "PASS"))
            this->Pass = all.substr(5, all.length());
        else if (!all.compare(0, 4, "NICK"))
            this->Nick = all.substr(5, all.length());
        else if (!all.compare(0, 4, "USER"))
            user_parse(all);
    }
}
void Client::user_parse(std::string user)
{
    std::string ret, garbage;
    std::stringstream ss(user);
    int i = 0;
    while (ss >> garbage)
    {
        if(i == 3)
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
Client::~Client()
{
}