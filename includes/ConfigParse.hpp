/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/13 19:32:47 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/25 12:59:56 by fbardeau         ###   ########.fr       */
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

#endif
