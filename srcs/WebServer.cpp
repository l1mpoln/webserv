/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServer.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/08 13:38:49 by vkuzmin           #+#    #+#             */
/*   Updated: 2024/02/01 18:16:41 by fbardeau         ###   ########.fr       */
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
    vector<ServerConfig>::iterator it;
    config.loadConfig(config_file_name);
    //signal(SIGINT, handleSignal);

    for(it = config.servers.begin(); it != config.servers.end(); ++it)
    {

        std::vector<std::string>::iterator portIt;
        for(portIt = it->_listenPorts.begin(); portIt != it->_listenPorts.end(); ++portIt) 
        {
            int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
            if (serverSocket == -1) {
                perror("socket");
                continue; // Ou gérer l'erreur d'une autre manière
            }

            //
            int yes = 1;
            if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
                perror("setsockopt");
                close(serverSocket); // S'assurer de fermer le socket en cas d'erreur
                continue;
            }
            //
            struct sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = INADDR_ANY;
            int port = atoi(portIt->c_str());
            serverAddress.sin_port = htons(port);
        
            bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
            cout << "Bind: " << serverSocket << " on port " << port << endl;
            listen(serverSocket, 100);
       
            cout << "Listening on http://" << it->_serverName << ":" << port << endl;
            
            socketToServerConfigMap[serverSocket] = *it;
            _fd.fd = serverSocket;
            _fd.events = POLLIN;
            listeningSocket.insert(serverSocket);
            fds.push_back(_fd);

            listeningPortMap[serverSocket] = port;
        }
    }
}

WebServer::~WebServer() 
{
    std::set<int>::iterator it;
    for (it = listeningSocket.begin(); it != listeningSocket.end(); ++it) {
        close(*it);
    }
    
}

void WebServer::start() {
   
    while (1) 
    {
        int result = poll(fds.data(), fds.size(), -1);
        if (result > 0) {
            for (size_t i = 0; i < fds.size(); ++i) 
            {
                if (fds[i].revents & POLLIN) 
                {
                    if (isListeningSocket(fds[i].fd))
                    {
                        int clientSocket = accept(fds[i].fd, NULL, NULL);
                        
                         socketToServerConfigMap[clientSocket] = socketToServerConfigMap[fds[i].fd];
                        unsigned int port = listeningPortMap[fds[i].fd];
                        
                        clientSocketToPortMap[clientSocket] = port;
                        _fd.fd = clientSocket;
                        _fd.events = POLLIN;
                        fds.push_back(_fd);
                    }
                    else 
                    {
                        char buffer[4096];
                        int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                        if (bytesRead > 0) {
                            i = checkClientMaxBodySize(buffer, bytesRead, i);
                        }
                        else 
                        {
                            close(fds[i].fd);
                            clientSocketToPortMap.erase(fds[i].fd);
                            fds.erase(fds.begin() + i);
                            --i;
                        }
                    }
                }
            }
        }
    }
}

int WebServer::checkClientMaxBodySize(char *buffer, int bytesRead, int i)
{
    std::string request(buffer, bytesRead);
    //
    if (!handleRequest(fds[i].fd, request))
        return i;
    //
    //int maxBodySize = atoi(config.get_clientMaxBodySize().c_str()) * 1000;
    int maxBodySize = atoi(socketToServerConfigMap[fds[i].fd]._clientMaxBodySize.c_str()) * 1000;
    size_t bodyPos = request.find("\r\n\r\n");
    std::string requestBody = (bodyPos != std::string::npos) ? request.substr(bodyPos + 4) : "";
    
    if (requestBody.size() > maxBodySize)
    {
        sendError413(fds[i].fd);
        close(fds[i].fd);
        clientSocketToPortMap.erase(fds[i].fd);
        fds.erase(fds.begin() + i);
        --i;
    }
    else 
    {
        std::cerr << request << std::endl;
        //handleRequest(fds[i].fd, request);
    }
    return i;
}

bool WebServer::isListeningSocket(int fd)
{
    return listeningSocket.find(fd) != listeningSocket.end();
}

bool isDirectory(const std::string& path) 
{
    struct stat statbuf;
    if (stat(path.c_str(), &statbuf) != 0)
        return false;
    return S_ISDIR(statbuf.st_mode);
}

bool WebServer::handleRequest(int clientSocket, const std::string& request) 
{
    std::istringstream requestStream(request);
    std::string method, path, version;
    vector<string>::iterator it;
    requestStream >> method >> path >> version;
    int get = 0; int post = 0; int del = 0 ;

    for (it = socketToServerConfigMap[clientSocket]._methods.begin(); it != socketToServerConfigMap[clientSocket]._methods.end(); ++it)
    {
        if (*it == "GET")
            get = 1;
        else if (*it == "POST")
            post = 1;
        else if (*it == "DELETE")
            del = 1;
    }
    if ((method == "GET" && get == 0) || (method == "POST" && post == 0) || (method == "DELETE" && del == 0))
    {
        cerr << "Method " << method << " not allowed" << endl;
        sendNotFoundResponse(clientSocket);
        return false;
    }
    if (method != "GET" && method != "POST" && method != "DELETE") {
        cout << "|" << method << "|" << endl;
        cout << "DEBUG_06\n";
        sendBadRequestResponse(clientSocket);
        return false;
    }
    if (path.find("..") != std::string::npos) {
        
        cout << "DEBUG_00\n";
        sendBadRequestResponse(clientSocket);
        
        return false;
    }
    // Version HTTP
    if (version != "HTTP/1.1") {
        sendBadRequestResponse(clientSocket);
        cout << "DEBUG_01\n";
        return false;
    }
    std::cout << "|" << socketToServerConfigMap[clientSocket]._index << "|" << endl;
    if (method == "GET") 
    {
        if (path == "/")
            sendFileResponse(clientSocket, socketToServerConfigMap[clientSocket]._index);
        else if (path == "/upload_checker")
            sendFileResponse(clientSocket, "data/upload_checker.html");
        else
        {
            if (isDirectory(path.substr(1)))
                sendFileResponse(clientSocket, socketToServerConfigMap[clientSocket]._index);
            else
                sendFileResponse(clientSocket, path.substr(1));
        }
    }
    else if (method == "POST") 
    {
        if (path == "/upload")
            handleFileUpload(clientSocket, requestStream);
        else
            sendTextResponse(clientSocket, "HTTP/1.1 200 OK\n");    //sendNotFoundResponse(clientSocket);
    } 
    else if (method == "DELETE") 
    {
        if (path == "/delete")
            sendTextResponse(clientSocket, "File deleted successfully");
        else
        {    
            cout << "DEBUG_02\n";
            sendNotFoundResponse(clientSocket);
        }
    } 
    else
        sendBadRequestResponse(clientSocket);
    return true;
}

void WebServer::sendFileResponse(int clientSocket, const std::string& filename) {
    std::ifstream file(filename.c_str());
    if (file.is_open()) 
    {
        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        unsigned int port = clientSocketToPortMap[clientSocket];
        std::ostringstream portStream;
        portStream << port;
        std::string portStr = portStream.str();

        std::string::size_type pos = content.find("{PORT}");
        while (pos != std::string::npos) {
            content.replace(pos, 6, portStr);
            pos = content.find("{PORT}", pos + portStr.length());
        }

        std::ostringstream responseStream;
        responseStream << "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: "
                       << content.size() << "\r\n\r\n" << content;
        std::string response = responseStream.str();

        send(clientSocket, response.c_str(), response.size(), 0);
        shutdown(clientSocket, SHUT_RDWR);
        close(clientSocket);
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
            sendError500(clientSocket);
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
    cerr << "Error 404: Page Not Found" << endl;
    std::string filename = socketToServerConfigMap[clientSocket]._error404;
    std::ifstream file(filename.c_str());
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
        file.close();
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void WebServer::sendBadRequestResponse(int clientSocket) 
{
    std::string filename = socketToServerConfigMap[clientSocket]._error400;
    std::ifstream file(filename.c_str());
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 400 Bad Request\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
        file.close();
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void WebServer::sendError413(int clientSocket) 
{
    std::string filename = socketToServerConfigMap[clientSocket]._error413;
    std::ifstream file(filename.c_str());
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 413 Payload Too Large\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
        file.close();
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}

void WebServer::sendError500(int clientSocket) 
{
    std::cerr << "Unable to open the output file.\n";
    std::string filename = socketToServerConfigMap[clientSocket]._error500;
    std::ifstream file(filename.c_str());
    if (file.is_open()) {
        std::ostringstream buffer;
        buffer << file.rdbuf();
        std::string content = buffer.str();
        std::string response = "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/html\r\nContent-Length: " + my_to_string(content.size()) + "\r\n\r\n" + content;
        send(clientSocket, response.c_str(), response.size(), 0);
        file.close();
    }
    shutdown(clientSocket, SHUT_RDWR);
    close(clientSocket);
}