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
#include "User.hpp"

class User;

class Channel
{
private:
	Channel(){};

public:
	Channel(std::string nm, std::string ky, bool hb) : name(nm), key(ky), isHub(hb){};
	~Channel(){};

	/*           VAR           */
	std::string	name;
	std::string	key;
	bool	isHub;
	std::vector<unsigned int>	members;
	std::vector<unsigned int>	ban_people;
	std::vector<unsigned int>	is_op;
	/*          ACTONS          */
	int		sendAll(std::string message, int ip, std::vector<User>::iterator& the);
	
};
