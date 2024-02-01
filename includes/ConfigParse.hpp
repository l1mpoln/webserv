/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:32:47 by fbardeau          #+#    #+#             */
/*   Updated: 2024/02/01 15:59:26 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSE_HPP
#define CONFIGPARSE_HPP

#include "WebServer.hpp"
#include "ServerConfig.hpp"

class ConfigParse
{
    public:
        std::vector<ServerConfig> servers;
        
        ConfigParse(){};
        ~ConfigParse(){};
        

        void loadConfig(const std::string& configFileName);
       
   
};
 ServerConfig check_config_file (ServerConfig currentServer);
#endif
