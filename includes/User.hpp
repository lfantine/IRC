#pragma once

#include "irc.hpp"

#define USER_DEFAULT_VALUE_STR "NONE_DEFAULT"
#define USER_DEFAULT_VALUE_INT -2

class Channel;
class Server;

class User
{
private:
	User(){};

	int	_id;
	std::string	_nickname;
	std::string	_username;
	std::string _real_name;
	int			_mode;
	std::string _mdp;
	bool connected;

public:
	User(int fd) : _id(fd), _nickname(USER_DEFAULT_VALUE_STR), _username(USER_DEFAULT_VALUE_STR), _real_name(USER_DEFAULT_VALUE_STR), _mode(USER_DEFAULT_VALUE_INT), _mdp(USER_DEFAULT_VALUE_STR), connected(false){};
	~User(){};

	std::vector<std::string> my_channel;

	int		getID() const;
	bool	isConnected() const;
	void	setConnect(bool status);
	int		setNickname(std::string name);
	std::string	getNickname() const;
	int		setRealname(std::string name);
	std::string	getRealname() const;
	int		setUsername(std::string name);
	std::string	getUsername() const;
	int		getMode() const;
	void	setMode(int mode);
	int		setMdp(std::string name);
	std::string	getMdp() const;

	// COMMAND

	void	USER(std::string line);
	void	NICK(std::string line, std::vector<User>& users);
	void	PASS(std::string line);
	void	JOIN(std::string line, Server& serv, std::vector<User>::iterator the);
	void	PART(std::string line, Server& serv);
	void	PRIVMSG(std::string line, Server& serv, int id);
	void	PING();
	void	NOTICE(std::string line, Server& serv);
};
