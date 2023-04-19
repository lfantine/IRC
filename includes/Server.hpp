#pragma once

#include "irc.hpp"

class User;
class Channel;

class Server
{
public:
	Server(void);
	Server(char *port, char *pass);
	Server(const Server & src);
	Server&	operator=(const Server & rhs);
	~Server();

	// GETTERS
	int	getServerFD() const;

	// ACTIONS
	int		CreateServer();
	int		StartServer();
	int		CCRoom(std::string name_, std::string key_, int id);
	int		LDRoom(std::string name_, int id);
	void	channelMess(std::string message, std::string channel, int id);

private:
	int			_port;
	std::string	_password;
	int			_socketFd;
	struct sockaddr_in	_addr;
	std::vector<struct pollfd> _fds;
	std::vector<User>	_Users;
	std::vector<Channel>	_Rooms;
	int			_timeout;
	bool		_ON;

	int			action(int ip, char* buffer, int len);
};
