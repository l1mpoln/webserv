/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuzmin <vkuzmin@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:35 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/11/11 15:33:11 by vkuzmin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <cstdlib>

class WebServer 
{
    public:
        WebServer();
        ~WebServer();

        void start();

    private:
        void handleRequest(int clientSocket, const std::string& request);
        void sendFileResponse(int clientSocket, const std::string& filename);
        void sendTextResponse(int clientSocket, const std::string& message);
        void sendNotFoundResponse(int clientSocket);
        void sendBadRequestResponse(int clientSocket);
        void handleFileUpload(int clientSocket, std::istringstream& requestStream);

    private:
        int serverSocket;
        std::vector<pollfd> fds;
};

#endif
