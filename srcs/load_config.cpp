/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:22:28 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/24 15:12:29 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/ConfigParse.hpp"

std::string extractValue(const std::string& line) 
{
    size_t firstQuote = line.find('"');
    size_t lastQuote = line.rfind('"');
    if (firstQuote != std::string::npos && lastQuote != std::string::npos && firstQuote != lastQuote) {
        return line.substr(firstQuote + 1, lastQuote - firstQuote - 1);
    }
    return "";
}


void ConfigParse::loadConfig(const std::string& configFileName) 
{
    std::ifstream configFile(configFileName.c_str());
    std::string line;

    // Loop through each line in the configuration file
    while (std::getline(configFile, line)) 
    {
        if (line.empty())
            continue;
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) 
            continue; // Pas de délimiteur '=' trouvé
        std::string key = trim(line.substr(0, delimiterPos));
       std::string value = extractValue(line);
       // std::string value = trim(line.substr(delimiterPos + 1));
        // std::cout << "Key: " << key << ", Value: " << value << std::endl; // Ajout d'une impression de débogage
        
        if (key == "server_name") {
            set_serverName(value);//serverName = value;
        } else if (key == "listen") {
            set_serverIP(value);//serverPort = std::stoi(value);
        } else if (key == "main") {
            set_mainPage(value);
        }else if (key == "error_404") {
            set_errorPage404(value); 
        }else if (key == "error_413") {
            set_errorPage413(value);
        } else if (key == "error_400") {
            set_errorPage400(value);
        } else if (key == "error_504") {
            set_errorPage504(value);
        } else if (key == "error_500") {
            set_errorPage500(value);
        }else if (key == "client_max_body_size") {
            set_clientMaxBodySize(value);
        } else if (key == "autoindex") {
            set_autoindex(value);
        } else if (key == "redirect") {
            set_redirect(value);
        } else if (key == "cgi") {
            set_cgi(value);
        } else if (key == "upload_path") {
            set_uploadPath(value);
        } else if (key == "deny") {
            set_deny(value);
        } else if (key == "allow") {
            set_allow(value);
        } else if (key == "max_clients") {
            set_maxClients(value); //= std::stoi(value); // Convert max_clients from string to integer
        } else if (key == "log") {
            set_logPath(value);
        } else if (key == "timeout") {
            set_timeout(value);// = std::stoi(value); // Convert timeout from string to integer
        }
    }
}

std::string trim(const std::string& str)
{
    
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}