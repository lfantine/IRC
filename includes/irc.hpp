#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <vector>
#include <string>
#include <sstream>
#include "Channel.hpp"
#include "User.hpp"
#include "Server.hpp"

#define MAXPORT 65535
#define TIMEOUT_M	5
#define TIMEOUT_S	30
#define Find_USER \
std::vector<User>::iterator the = this->_Users.begin();\
	for (size_t i = 0; i < this->_Users.size(); i++)\
	{\
		if (the->getID() == ip)\
			break ;\
		the++;\
	}

int	mySend(std::string mess, int ip);
std::string	replyMess(std::string message, std::vector<User>::iterator& the, std::string channel);
std::string	replyWelcome(std::vector<User>::iterator& the);
std::string	replyJOIN(std::string channel, std::vector<User>::iterator& the);
std::string	replyPART(std::string channel, std::vector<User>::iterator& the);
std::string	ERR_NEEDMOREPARAMS(std::string cmd);
std::string	ERR_ALREADYREGISTRED();
std::string	ERR_NONICKNAMEGIVEN();
std::string	ERR_ERRONEUSNICKNAME(std::string nick);
std::string	ERR_NICKNAMEINUSE(std::string nick);
std::string	ERR_BANNEDFROMCHAN(std::string channel);
std::string	ERR_BADCHANNELKEY(std::string channel);