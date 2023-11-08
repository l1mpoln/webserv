/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuzmin <vkuzmin@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:49 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/11/08 14:03:16 by vkuzmin          ###   ########.fr       */
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

void WebServer::handleRequest(int clientSocket, const std::string& request) 
{
    std::istringstream requestStream(request);
    std::string method, path, version;
    requestStream >> method >> path >> version;

    if (method == "GET") {
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

void WebServer::handleFileUpload(int clientSocket, std::istringstream& requestStream) {
    // Обработка загрузки файла
    // В этой функции вы можете прочитать тело запроса и сохранить файл на сервере.
    // Пример работы с загрузкой файлов не предоставляется в данном ответе.
}
  