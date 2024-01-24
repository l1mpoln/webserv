/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParse.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fbardeau <fbardeau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/04 13:23:54 by fbardeau          #+#    #+#             */
/*   Updated: 2024/01/23 09:35:04 by fbardeau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/WebServer.hpp"
#include "../includes/ConfigParse.hpp"

void ConfigParse::set_serverName(std::string serverName){
	this->_serverName = serverName;
}
void ConfigParse::set_mainPage(std::string mainPage){
	this->_mainPage = mainPage;
}
void ConfigParse::set_errorPage404(std::string error404){
	this->_errorPage404 = error404;
}
void ConfigParse::set_errorPage403(std::string error403){
	this->_errorPage403 = error403;
}
void ConfigParse::set_errorPage504(std::string error504){
	this->_errorPage504 = error504;
}
void ConfigParse::set_serverIP(std::string serverIP){
	this->_serverIP = serverIP;
}
void ConfigParse::set_clientMaxBodySize(std::string clientMaxBdySize){
	this->_clientMaxBodySize = clientMaxBdySize;
}
void ConfigParse::set_autoindex(std::string autoindex){
	this->_autoindex = autoindex;
}
void ConfigParse::set_redirect(std::string redirect){
	this->_redirect = redirect;
}
void ConfigParse::set_cgi(std::string path_cgi){
	this->_cgi = path_cgi;
}
void ConfigParse::set_uploadPath(std::string upload_path){
	this->_uploadPath = upload_path;
}
void ConfigParse::set_deny(std::string deny){
	this->_deny = deny;
}
void ConfigParse::set_allow(std::string allow){
	this->_allow = allow;
}
void ConfigParse::set_maxClients(std::string maxClients){
	this->_maxClients = maxClients;
}
void ConfigParse::set_logPath(std::string log_Path){
	this->_logPath = log_Path;
}
void ConfigParse::set_timeout(std::string timeout){
	this->_timeout = timeout;
}

std::string ConfigParse::get_serverName(){
	return this->_serverName;
}

std::string ConfigParse::get_mainPage(){
	return this->_mainPage; 
}

std::string ConfigParse::get_errorPage404(){
	return this->_errorPage404;
}

std::string ConfigParse::get_errorPage403(){
	return this->_errorPage403;
}

std::string ConfigParse::get_errorPage504(){
	return this->_errorPage504;
}

std::vector<unsigned int> ConfigParse::get_serverIP()
{
 	std::vector<unsigned int> validIPs;
	std::string ip;
	
	for (int i = 0; _serverIP[i] != '\0'; ++i)
	{
		if (isdigit(_serverIP[i]))
		{
			ip = ip + _serverIP[i];
		}
		else
		{	
			if (!ip.empty()){	
				validIPs.push_back(static_cast<unsigned int>(std::atoi(ip.c_str())));
				ip.clear();
			}
		}
	}
	if (!ip.empty())
				validIPs.push_back(static_cast<unsigned int>(std::atoi(ip.c_str())));
    // Afficher les adresses IP valides
   /* std::cout << "Adresses IP valides :" << std::endl;
	
    for (std::vector <unsigned int>::iterator it = validIPs.begin(); it != validIPs.end(); ++it) 
	{
        std::cout << *it << std::endl;
    }*/
	return validIPs;
}

std::string ConfigParse::get_clientMaxBodySize(){
	
	std::string BodySize;
	
	for (int i = 0; this->_clientMaxBodySize[i]; ++i)
	{
		if (isdigit(this->_clientMaxBodySize[i]))
		{
			BodySize += _clientMaxBodySize[i];
		}
	}
	return BodySize;
}

std::string ConfigParse::get_autoindex(){
	return this->_autoindex;
}

std::string ConfigParse::get_redirect(){
	return this->_redirect;
}

std::string ConfigParse::get_cgi(){
	return this->_cgi;
}

std::string ConfigParse::get_uploadPath(){
	return this->_uploadPath;
}

std::string ConfigParse::get_deny(){
	return this->_deny;
}

std::string ConfigParse::get_allow(){
	return this->_allow;
}

std::string ConfigParse::get_maxClients(){
	return this->_maxClients;
}

std::string ConfigParse::get_logPath(){
	return this->_logPath;
}

std::string ConfigParse::get_timeout(){
	return this->_timeout;
}
