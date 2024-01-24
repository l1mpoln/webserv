/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:32:47 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/22 15:30:39 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSE_HPP
#define CONFIGPARSE_HPP

#include "WebServer.hpp"

class ConfigParse
{
    private:
        std::string _serverName;
        std::string _mainPage;
        std::string _errorPage404;
        std::string _errorPage403;
        std::string _errorPage504;
        std::string _serverIP;
        std::string _clientMaxBodySize;
        std::string _autoindex;
        std::string _redirect;
        std::string _cgi;
        std::string _uploadPath;
        std::string _deny;
        std::string _allow;
        std::string _maxClients;
        std::string _logPath;
        std::string _timeout;
        
    public:
        ConfigParse(){};
        ~ConfigParse(){};
        
        void set_serverName(std::string serverName);
        void set_mainPage(std::string mainPage);
        void set_errorPage404(std::string error404);
        void set_errorPage403(std::string error403);
        void set_errorPage504(std::string error504);
        void set_serverIP(std::string serverIP);
        void set_clientMaxBodySize(std::string clientMaxBdySize);
        void set_autoindex(std::string autoindex);
        void set_redirect(std::string redirect);
        void set_cgi(std::string path_cgi);
        void set_uploadPath(std::string upload_path);
        void set_deny(std::string deny);
        void set_allow(std::string allow);
        void set_maxClients(std::string maxClients);
        void set_logPath(std::string log_Path);
        void set_timeout(std::string timeout);
        
        std::string get_serverName();
        std::string get_mainPage();
        std::string get_errorPage404();
        std::string get_errorPage403();
        std::string get_errorPage504();
        std::vector<unsigned int> get_serverIP();
        std::string get_clientMaxBodySize();
        std::string get_autoindex();
        std::string get_redirect();
        std::string get_cgi();
        std::string get_uploadPath();
        std::string get_deny();
        std::string get_allow();
        std::string get_maxClients();
        std::string get_logPath();
        std::string get_timeout();

        void loadConfig(const std::string& configFileName);
   
};

#endif
