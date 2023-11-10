/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vkuzmin <vkuzmin@student.42nice.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 22:26:40 by vkuzmin           #+#    #+#             */
/*   Updated: 2023/11/10 17:59:44 by vkuzmin          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"

int main() {
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    listen(serverSocket, 5);

    std::vector<pollfd> fds(1);
    fds[0].fd = serverSocket;
    fds[0].events = POLLIN;

    WebServer server;

    while (1) {
        int result = poll(fds.data(), fds.size(), -1);

        if (result > 0) {
            if (fds[0].revents & POLLIN) {
                int clientSocket = accept(serverSocket, NULL, NULL);
                fds.push_back({clientSocket, POLLIN});
            }

            for (size_t i = 1; i < fds.size(); ++i) {
                if (fds[i].revents & POLLIN) {
                    char buffer[1024];
                    int bytesRead = recv(fds[i].fd, buffer, sizeof(buffer), 0);
                    if (bytesRead > 0) {
                        std::string request(buffer, bytesRead);
                        server.handleRequest(fds[i].fd, request);
                    } else {
                        close(fds[i].fd);
                        fds.erase(fds.begin() + i);
                        --i;
                    }
                }
            }
        }
    }

    return 0;
}