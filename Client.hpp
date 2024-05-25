/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abenmous <abenmous@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 08:26:12 by abenmous          #+#    #+#             */
/*   Updated: 2024/05/25 10:43:54 by abenmous         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include"Server.hpp"

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
        void erase_attr();
        int get_fd();
        void set_fd(int fd);
        void set_user(std::string u);
        void set_nick(std::string n);
        ~Client();
};