#include "../includes/irc.hpp"

#define IP_ADDR "127.0.0.1"

std::string	replyMess(std::string message, std::vector<User>::iterator& the, std::string channel){
	std::stringstream 	ss;
	ss << ":" << the->getNickname() <<  "!" << the->getUsername() << "@" << IP_ADDR << " PRIVMSG "<< channel << " :" << message << "\r\n";
	return ss.str();
}

std::string	replyPONG(){
	std::stringstream 	ss;
	ss << "PONG " << IP_ADDR << "\r\n";
	return ss.str();
}

std::string replyNOTICE(std::string message, std::vector<User>::iterator& the){
	std::stringstream 	ss;
	ss << ":" << the->getNickname() <<  "!" << the->getUsername() << "@" << IP_ADDR << " NOTICE s " << message << "\r\n";
	return ss.str();
}

std::string	replyWelcome(std::vector<User>::iterator& the){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 001 " << the->getNickname() << " Welcome to the CHAT " << the->getNickname() << "!" << the->getUsername() << "@" << IP_ADDR << "\r\n";
	return ss.str();
}

std::string	replyJOIN(std::string channel, std::vector<User>::iterator& the){
	std::stringstream 	ss;
	ss << ":" << the->getNickname() << "!" << the->getUsername() << "@" << IP_ADDR << " JOIN " << channel << "\r\n";
	return ss.str();
}

std::string	replyPART(std::string channel, std::vector<User>::iterator& the){
	std::stringstream 	ss;
	ss << ":" << the->getNickname() << "!" << the->getUsername() << "@" << IP_ADDR << " PART " << channel << "\r\n";
	return ss.str();
}

std::string	ERR_NEEDMOREPARAMS(std::string cmd){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 461 " << cmd << ":Not enough parameters" << "\r\n";
	return ss.str();
}

std::string	ERR_ALREADYREGISTRED(){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 462 " << ":Unauthorized command (already registered)" << "\r\n";
	return ss.str();
}

std::string	ERR_NONICKNAMEGIVEN(){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 431 " << ":No nickname given" << "\r\n";
	return ss.str();
}

std::string	ERR_ERRONEUSNICKNAME(std::string nick){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 432 " << nick << " :Erroneous nickname" << "\r\n";
	return ss.str();
}

std::string	ERR_NICKNAMEINUSE(std::string nick){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 433 " << nick << " :Nickname is already in use" << "\r\n";
	return ss.str();
}

std::string	ERR_BANNEDFROMCHAN(std::string channel){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 474 " << channel << " :cannot join channel" << "\r\n";
	return ss.str();
}

std::string	ERR_BADCHANNELKEY(std::string channel){
	std::stringstream 	ss;
	ss << ":" << IP_ADDR << " 475 " << channel << " :cannot join channel" << "\r\n";
	return ss.str();
}
