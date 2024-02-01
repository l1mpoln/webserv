/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:35 by vkuzmin           #+#    #+#             */
/*   Updated: 2024/02/01 18:13:22 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERVER_HPP
#define WEBSERVER_HPP

#include <iostream>
#include <sys/stat.h>
#include <fstream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <vector>
#include <cstdlib>
#include <set>
#include <map>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <csignal>
#include "../includes/ConfigParse.hpp"
#include "../includes/ServerConfig.hpp"
using namespace std; 



class WebServer
{
    public:
        WebServer(std::string config_file_name);
        ~WebServer();
        void start();

    private:
        bool handleRequest(int clientSocket, const std::string& request);
        void sendFileResponse(int clientSocket, const std::string& filename);
        void sendTextResponse(int clientSocket, const std::string& message);
        void sendNotFoundResponse(int clientSocket);
        void sendBadRequestResponse(int clientSocket);
        void handleFileUpload(int clientSocket, std::istringstream& requestStream);
        bool isListeningSocket(int fd);
        int checkClientMaxBodySize(char *buffer, int bytesRead, int i);
        void sendError413(int clientSocket);
        void sendError500(int clientSocket);
        
    
    private:
        ConfigParse config;
        std::map<int, ServerConfig> socketToServerConfigMap;
        
        std::set<int> listeningSocket;
        int serverPort;  //For config
        std::string defaultPage;//For config
        std::string error_page404;//For config
        
        
        int serverSocket;
        std::vector<pollfd> fds;
        pollfd _fd;
        std::map<int, unsigned int> listeningPortMap;
        std::map<int, unsigned int> clientSocketToPortMap;

};

//void handleSignal(int signal);
std::string trim(const std::string& str);


#endif
