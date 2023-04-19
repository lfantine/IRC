#include "../includes/User.hpp"

int	User::setNickname(std::string name){
	if (name.size() > 1){
		_nickname = name;
		return 1;
	}
	return 0;
}

int	User::getID() const{
	return this->_id;
}

bool	User::isConnected() const{
	return this->connected;
}

void	User::setConnect(bool status){
	this->connected = status;
}

std::string	User::getNickname() const{
	return this->_nickname;
}

std::string	User::getRealname() const{
	return this->_real_name;
}

int	User::setRealname(std::string name){
	if (name.size() > 1){
		_real_name = name;
		return 1;
	}
	return 0;
}

int	User::getMode() const{
	return this->_mode;
}

void	User::setMode(int mode){
	this->_mode = mode;
}

std::string	User::getUsername() const{
	return this->_username;
}

int	User::setUsername(std::string name){
	if (name.size() > 1){
		_username = name;
		return 1;
	}
	return 0;
}

std::string	User::getMdp() const{
	return this->_mdp;
}

int	User::setMdp(std::string name){
	if (name.size() > 1){
		_mdp = name;
		return 1;
	}
	return 0;
}



/*///////////   COMMANDE   ////////////*/

void	User::USER(std::string line){
	if (line.size() < 11){
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	int i = 5;
	std::string	newNick = "";
	std::string newReal = "";
	int			mode = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\n' || !std::isprint(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (line[i] != ' ' || line[i] == '\n' || !std::isprint(line[i]))
	{
		newNick.push_back(line[i]);
		i++;
	}
	if (line[i] == '\n' || !std::isprint(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (line[i] == ' ')
		i++;
	if (!std::isdigit(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (std::isdigit(line[i]))
	{
		mode *= 10;
		mode += line[i] - 48;
		i++;
	}
	if (line[i] == '\n' || !std::isprint(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (line[i] == ' ')
		i++;
	if (line[i++] != '*')
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	if (line[i] == '\n' || !std::isprint(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (line[i] == ' ')
		i++;
	if (line[i++] != ':')
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	if (line[i] == '\n' || !std::isprint(line[i]))
	{
		mySend(ERR_NEEDMOREPARAMS("USER"), this->getID());
		return ;
	}
	while (line[i] != '\n' || std::isprint(line[i]))
	{
		newReal.push_back(line[i]);
		i++;
	}
	this->setUsername(newNick);
	this->setRealname(newReal);
	this->setMode(mode);
	return ;
}

void	User::NICK(std::string line, std::vector<User>& users){
	if (line.size() < 6){
		mySend(ERR_NONICKNAMEGIVEN(), this->getID());
		return ;
	}
	int	l = 0;
	for (size_t i = 5; i < line.size()-1; i++)
	{
		if (std::isalnum(line[i]))
		{
			l = i;
			break ;
		}
	}
	if (l > 0)
	{
		std::string new_name; // = tab[0].substr(l, tab[0].size()-l-1);
		int	t = 0;
		for (size_t i = l; i < line.size() && line[i] != '\r' && line[i] != '\n'; i++){
			new_name.push_back(line[i]);
			if (!std::isprint(line[i])){
				mySend(ERR_ERRONEUSNICKNAME(new_name), this->getID());
				return ;
			}
			t++;
		}
		if (t < 5){
			mySend(ERR_ERRONEUSNICKNAME(new_name), this->getID());
			return ;
		}
		for (size_t i = 0; i < users.size(); i++){
			if (new_name == users[i].getNickname()){
				mySend(ERR_NICKNAMEINUSE(new_name), this->getID());
			return ;
			}
		}
		
		this->setNickname(new_name);
	}
	else
	{
		mySend(ERR_NONICKNAMEGIVEN(), this->getID());
		return ;
	}
}

void	User::PASS(std::string line){
	if (line.size() < 6){
		mySend(ERR_NEEDMOREPARAMS("PASS"), this->getID());
		return ;
	}
	std::string tpass;
	int	i = 5;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\n' || !std::isprint(line[i])){
		mySend(ERR_NEEDMOREPARAMS("PASS"), this->getID());
		return ;
	}
	while (line[i] != '\n' && std::isprint(line[i]))
	{
		tpass.push_back(line[i]);
		i++;
	}
	this->setMdp(tpass);
}

void	User::JOIN(std::string line, Server& serv){
	if (line.size() < 6){
		mySend(ERR_NEEDMOREPARAMS("JOIN"), this->getID());
		return ;
	}
	std::string name;
	std::string key = "";
	int	i = 5;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\n' || !std::isprint(line[i])){
		mySend(ERR_NEEDMOREPARAMS("JOIN"), this->getID());
		return ;
	}
	while (line[i] != '\n' && std::isprint(line[i]) && line[i] != ' ')
	{
		name.push_back(line[i]);
		i++;
	}
	while (line[i] == ' ')
		i++;
	while (line[i] != '\n' && std::isprint(line[i]))
	{
		key.push_back(line[i]);
		i++;
	}
	if (serv.CCRoom(name, key, this->getID()))
	{
		for (size_t i = 0; i < this->my_channel.size(); i++){
			if (this->my_channel[i] == name)
				return ;
		}
		this->my_channel.push_back(name);
	}
}

void	User::PART(std::string line, Server& serv){
	if (line.size() < 6){
		mySend(ERR_NEEDMOREPARAMS("PART"), this->getID());
		return ;
	}
	std::string name;
	int	i = 5;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\n' || !std::isprint(line[i])){
		mySend(ERR_NEEDMOREPARAMS("PART"), this->getID());
		return ;
	}
	while (line[i] != '\n' && std::isprint(line[i]) && line[i] != ' ')
	{
		name.push_back(line[i]);
		i++;
	}
	serv.LDRoom(name, this->getID());
	std::vector<std::string>::iterator sit = this->my_channel.begin();
	for (size_t i = 0; i < this->my_channel.size(); i++)
	{
		if (this->my_channel[i] == name)
		{
			this->my_channel.erase(sit);
			return ;
		}	
		sit++;
	}
}

void	User::PRIVMSG(std::string line, Server& serv, int id){
	int i = 7;
	std::string target;
	std::string message;
	while (line[i] == ' ')
		i++;
	while (std::isalpha(line[i]))
	{
		target.push_back(line[i]);
		i++;
	}
	while (line[i] == ' ')
		i++;
	if (line[i++] != ':')
		return ;
	while (std::isalpha(line[i]) || line[i] == ' '){
		message.push_back(line[i]);
		i++;
	}
	serv.channelMess(message, target, id);
}