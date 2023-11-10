/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuzmin <vkuzmin@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:49 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/11/10 20:28:15 by vkuzmin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

template <typename T>
std::string my_to_string(T value) 
{
    std::ostringstream os;
    os << value;
    return os.str();
}

WebServer::WebServer() 
{
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    fds.push_back({serverSocket, POLLIN});
}

WebServer::~WebServer() 
{
    close(serverSocket);
}

void WebServer::start() 
{
    while (1) {
        int result = poll(fds.data(), fds.size(), -1);

        if (result > 0) 
        {
            if (fds[0].revents & POLLIN) 
            {
                int clientSocket = accept(serverSocket, NULL, NULL);
                fds.push_back({clientSocket, POLLIN});
            }

            for (size_t i = 1; i < fds.size(); ++i) 
            {
                if (fds[i].revents & POLLIN) 
                {
                    char buffer[1024];
                    int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (bytesRead > 0) 
                    {
                        std::string request(buffer, bytesRead);
                        handleRequest(fds[i].fd, request);
                    } 
                    else 
                    {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                }
            }
        }
    }
}

void WebServer::handleRequest(int clientSocket, const std::string& request) 
{
    std::istringstream requestStream(request);
    std::string method, path, version;
    requestStream >> method >> path >> version;

    if (method == "GET") 
    {
        if (path == "/")
            sendFileResponse(clientSocket, "data/index.html");
        else if (path == "/hello")
            sendTextResponse(clientSocket, "Hello, World!");
        else
            sendNotFoundResponse(clientSocket);
    } 
    else if (method == "POST") 
    {
        if (path == "/upload")
            handleFileUpload(clientSocket, requestStream);
        else
            sendNotFoundResponse(clientSocket);
    } 
    else if (method == "DELETE") 
    {
        if (path == "/delete")
            sendTextResponse(clientSocket, "File deleted successfully");
        else
            sendNotFoundResponse(clientSocket);
    } 
    else
        sendBadRequestResponse(clientSocket);
}

void WebServer::sendFileResponse(int clientSocket, const std::string& filename) 
{
    std::ifstream file(filename.c_str());
    if (file.is_open()) 
    {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
    } 
    else 
    {
        sendNotFoundResponse(clientSocket);
    }
}

void WebServer::sendTextResponse(int clientSocket, const std::string& message) 
{
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + my_to_string(message.size()) + "\r\n\r\n" + message;
    send(clientSocket, response.c_str(), response.size(), 0);
}

void WebServer::sendNotFoundResponse(int clientSocket) 
{
    std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<html><body><h1>404 Not Found</h1></body></html>";
    send(clientSocket, response.c_str(), response.size(), 0);
}

void WebServer::sendBadRequestResponse(int clientSocket) 
{
    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<html><body><h1>400 Bad Request</h1></body></html>";
    send(clientSocket, response.c_str(), response.size(), 0);
}

void WebServer::handleFileUpload(int clientSocket, std::istringstream& requestStream) 
{
    // Обработка загрузки файла
    // В этой функции вы можете прочитать тело запроса и сохранить файл на сервере.
    // Пример работы с загрузкой файлов не предоставляется в данном ответе.
}
