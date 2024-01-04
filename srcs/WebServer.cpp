/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:49 by vkuzmin           #+#    #+#             */
/*   Updated: 2024/01/04 15:24:43 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/ConfigParse.hpp"

using namespace std;

template <typename T>
std::string my_to_string(T value) 
{
    std::ostringstream os;
    os << value;
    return os.str();
}

WebServer::WebServer(std::string config_file_name) 
{
    vector<int> Ports;
    vector<int>::iterator it;
    config.loadConfig(config_file_name);
    ////
    cout <<"ser : |" << config.get_serverName() << "|" << endl;
    cout <<"mai : |" << config.get_mainPage() << "|" << endl;
    cout <<"err : |" << config.get_errorPage404() << "|" << endl;
    cout <<"err : |" << config.get_errorPage403() << "|" << endl;
    cout <<"err : |" << config.get_errorPage504() << "|" << endl;
    cout <<"ser : |" << config.get_serverIP() << "|" << endl;
    cout <<"cli : |" << config.get_clientMaxBodySize() << "|" << endl;
    cout <<"aut : |" << config.get_autoindex() << "|" << endl;
    cout <<"red : |" << config.get_redirect() << "|" << endl;
    cout <<"cgi : |" << config.get_cgi() << "|" << endl;
    cout <<"upl : |" << config.get_uploadPath() << "|" << endl;
    cout <<"den : |" << config.get_deny() << "|" << endl;
    cout <<"all : |" << config.get_allow() << "|" << endl;
    cout <<"max : |" << config.get_maxClients() << "|" << endl;
    cout <<"log : |" << config.get_logPath() << "|" << endl;
    cout <<"tim : |" << config.get_timeout() << "|" << endl;
    

    ////
    Ports.push_back(8080);
    Ports.push_back(3000);
    for (it = Ports.begin(); it != Ports.end(); ++it) // check multi port
    {
        int port = *it;
        serverSocket = socket(AF_INET, SOCK_STREAM, 0);

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        serverAddress.sin_port = htons(port);
    
        bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        cout << "Bind : " << serverSocket << endl;
        listen(serverSocket, atoi(config.get_maxClients().c_str()));
        cout << "Listening on http://localhost:" << port << endl;
    
        _fd.fd = serverSocket;
        _fd.events = POLLIN;
        listeningSocket.insert(serverSocket);
        fds.push_back(_fd);
        //fds.push_back({serverSocket, POLLIN}); // c++11
    }
}

WebServer::~WebServer() 
{
    close(serverSocket);
}

void WebServer::start() {
    while (1) {
        int result = poll(fds.data(), fds.size(), -1);

        if (result > 0) 
        {
            for (size_t i = 0; i < fds.size(); ++i) 
            {
                if (fds[i].revents & POLLIN)
                {
                    if (isListeningSocket(fds[i].fd)) 
                    { // check if the socket listen
                        int clientSocket = accept(fds[i].fd, NULL, NULL);
                        _fd.fd = clientSocket;
                        _fd.events = POLLIN;
                        fds.push_back(_fd);
                    } 
                    else 
                    {    // check client request
                        char buffer[1024];
                        int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                        if (bytesRead > 0) {
                            std::string request(buffer, bytesRead);
                            std::cerr << request << std::endl;
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
}

bool WebServer::isListeningSocket(int fd)
{
    return listeningSocket.find(fd) != listeningSocket.end();
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
        else if (path == "/upload_checker")
            sendFileResponse(clientSocket, "data/upload_checker.html");
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

void WebServer::handleFileUpload(int clientSocket, std::istringstream& requestStream) 
{
    int contentLength = 0;
    std::string line;
    
    while (std::getline(requestStream, line)) 
    {
        if (line == "\r") 
            break;
        else if (line.find("Content-Length:") != std::string::npos) 
            contentLength = atoi(line.substr(line.find(":") + 1).c_str());
    }
    std::cerr << "Content-Length: " << contentLength << std::endl;
    std::ostringstream fileData;
    char buffer[1024];
    int bytesRead = 0;
    int totalBytesRead = 0;
    while (totalBytesRead < contentLength) 
    {
        bytesRead = recv(clientSocket, buffer, (sizeof(buffer) < (contentLength - totalBytesRead)) ? sizeof(buffer) : (contentLength - totalBytesRead), 0);
        if (bytesRead <= 0) 
        {
            std::cerr << "Error reading file data.\n";
            std::string response = "HTTP/1.1 500 Internal Server Error\r\n\r\nFailed to read file data.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
            shutdown(clientSocket, SHUT_RDWR);
            close(clientSocket);
            return;
        }
        totalBytesRead += bytesRead;
        fileData.write(buffer, bytesRead);
    }
    if (totalBytesRead == contentLength) 
    {
        std::ofstream outputFile("./Upload_File/uploaded_file.txt", std::ios::binary);
        if (outputFile.is_open()) 
        {
            std::cerr << "IN the file upload4" << std::endl;
            outputFile << fileData.str();
            outputFile.close();
            std::cerr << "File uploaded successfully.\n";
            std::string response = "HTTP/1.1 200 OK\r\n\r\nFile uploaded successfully.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        } 
        else 
        {
            std::cerr << "Unable to open the output file.\n";
            std::string response = "HTTP/1.1 500 Internal Server Error\r\n\r\nFailed to open the output file.\n";
            send(clientSocket, response.c_str(), response.size(), 0);
        }
    }

    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void WebServer::sendTextResponse(int clientSocket, const std::string& message) 
{
    std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: " + my_to_string(message.size()) + "\r\n\r\n" + message;
    send(clientSocket, response.c_str(), response.size(), 0);
}


void WebServer::sendNotFoundResponse(int clientSocket)
{
    std::string filename = "./Error_Page/404.html";
    std::ifstream file(filename.c_str());
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
        file.close();
    }
}

void WebServer::sendBadRequestResponse(int clientSocket) 
{
    std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\n\r\n<html><body><h1>400 Bad Request</h1></body></html>";
    send(clientSocket, response.c_str(), response.size(), 0);
}

