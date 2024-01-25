/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 12:56:38 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/25 16:47:24 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "ConfigParse.hpp"

class ServerConfig
{
    public:
		std::string _serverName;
    	std::vector<std::string> _listenPorts;
    	bool _autoindex;
    	std::vector<std::string> _methods;
    	std::string _defaultPage;
    	std::string _clientMaxBodySize;
    	std::string _maxClients;
    	std::string _index; 
		std::string _upload;
		std::string _error400;
		std::string _error403;
		std::string _error404;
		std::string _error413;
		std::string _error500;
		std::string _error504;
		std::string _script1;
		std::string _script2;
		std::string _script3;
    
     	ServerConfig(){};
        ~ServerConfig(){};
};

#endif