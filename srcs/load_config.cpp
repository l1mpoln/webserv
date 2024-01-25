/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/05 16:22:28 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/25 18:05:30 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/ConfigParse.hpp"

/*std::string extractValue(const std::string& line) 
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
}*/


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
    ServerConfig currentServer;
    bool inServerBlock = false;
    int count = 0;

    while (std::getline(configFile, line)) 
    {
        if (line.empty())
            continue;
        if (line.find("server {") != std::string::npos)
        {
            if (count > 0)
                servers.push_back(currentServer);
            count++;
            currentServer = ServerConfig();
        }
        size_t delimiterPos = line.find('=');
        if (delimiterPos == std::string::npos) 
            continue;
        std::string key = trim(line.substr(0, delimiterPos));
        std::string value = extractValue(line);
        if (key == "server_name") {
            currentServer._serverName = value;
        } 
        else if (key == "listen") {
            std::istringstream iss(value);
            std::string port;
            while (getline(iss, port, ' ')) {
                currentServer._listenPorts.push_back(port);
            }
        } else if (key == "autoindex") {
            currentServer._autoindex = (value == "on");
        } else if (key == "methods") {
            std::istringstream iss(value);
            std::string method;
            while (getline(iss, method, ' ')) {
                currentServer._methods.push_back(method);
            }
        } else if (key == "default") {
                currentServer._defaultPage = value;
        }
        else if (key == "client_max_body_size") 
        {
	        for (int i = 0; value[i]; ++i)
	        {
	        	if (isdigit(value[i]))
	        	{
	        		currentServer._clientMaxBodySize += value[i];
	        	}
	        }
        } 
        else if (key == "max_clients") {
            currentServer._maxClients = value;
        }
        else if (key == "index") {
             currentServer._index = value;
        }
        else if (key == "upload"){
            currentServer._upload = value;
        }
        else if (key == "error400"){
            currentServer._error400 = value;
        }
        else if(key == "error403"){
            currentServer._error403 = value;
        }
        else if(key == "error404"){
            currentServer._error404 = value;
        }
        else if(key == "error413"){
            currentServer._error413 = value;
        }
        else if(key == "error500"){
            currentServer._error500 = value;
        }
        else if(key == "error504"){
             currentServer._error504 = value;
         }
        else if(key == "script1"){
            currentServer._script1 = value;
        }
        else if(key == "script2"){
            currentServer._script2 = value;
        }
        else if(key == "script3"){
            currentServer._script3 = value;
        }
    }
    servers.push_back(currentServer);
}

std::string trim(const std::string& str)
{
    
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos)
        return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}