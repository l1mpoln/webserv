/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuzmin <vkuzmin@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:35 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/11/08 13:45:01 by vkuzmin          ###   ########.fr       */
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

class WebServer {
public:
    void handleRequest(int clientSocket, const std::string& request);
    void sendFileResponse(int clientSocket, const std::string& filename);
    void sendTextResponse(int clientSocket, const std::string& message);
    void sendNotFoundResponse(int clientSocket);
    void sendBadRequestResponse(int clientSocket);
    void handleFileUpload(int clientSocket, std::istringstream& requestStream);
};

#endif
