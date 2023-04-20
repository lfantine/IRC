#include "../includes/irc.hpp"

#define LINE tab[0]


/*########## fct #############*/
int		verif_connect(std::vector<User>::iterator& the, std::string serv_pass);
/*-------------------------------*/

int	mySend(std::string mess, int ip){
	return (send(ip, &mess[0], mess.size(), 0));
}

bool	strFind(std::string str, std::string buf){
	for (size_t i = 0; str[i]; i++)
	{
		if (buf[i] != str[i])
			return false;
	}
	return true;
}

/*--------------[ Constructor ]-----------------*/

Server::Server()
{

}

Server::Server(char *port, char *pass) : _port(std::stoi(port)), _password(pass)
{
	//Channel	main("HUB");
	//this->_Rooms.push_back(main);
}

Server::Server(const Server & src)
{
	*this=src;
}

Server&	Server::operator=(const Server & rhs)
{
	this->_port = rhs._port;
	this->_password = rhs._password;
	this->_socketFd = rhs._socketFd;
	return (*this);
}

Server::~Server()
{

}

//*--------------[ Actions ]-----------------*//

int	Server::getServerFD() const
{
	return (this->_socketFd);
}

//*--------------[ Actions ]-----------------*//

int	Server::CCRoom(std::string name_, std::string key_, int id){
	int ip = id;
	Find_USER
	for (size_t i = 0; i < this->_Rooms.size(); i++)
	{
		//JOIN
		if (this->_Rooms[i].name == name_)
		{
			for (size_t j = 0; j < this->_Rooms[i].ban_people.size(); j++)
			{
				if (this->_Rooms[i].ban_people[j] == static_cast<unsigned int>(id))
				{
					mySend(ERR_BANNEDFROMCHAN(name_), id);
					return 0;
				}
			}
			if ((!this->_Rooms[i].isHub && this->_Rooms[i].key == key_) || this->_Rooms[i].isHub)
			{
				this->_Rooms[i].members.push_back(id);
				for (size_t h = 0; h < this->_Rooms[i].members.size(); h++)
					mySend(replyJOIN(name_, the), this->_Rooms[i].members[h]);
				for (std::vector<User>::iterator uit = this->_Users.begin(); uit != this->_Users.end(); uit++)
				{
					if (uit->getID() != id)
						mySend(replyJOIN(name_, uit), id);
				}
			}
			else if (!this->_Rooms[i].isHub && this->_Rooms[i].key != key_)
			{
				mySend(ERR_BADCHANNELKEY(name_), id);
				return 0;
			}
			return 1;
		}
	}
	// CREATE
	Channel	chanL(name_, key_, true);
	if(key_.size() > 0)
		chanL.isHub = false;
	chanL.members.push_back(id);
	this->_Rooms.push_back(chanL);
	mySend(replyJOIN(name_, the), id);
	return 1;
}

int	Server::LDRoom(std::string name_, int id){
	int ip = id;
	Find_USER
	for (size_t i = 0; i < this->_Rooms.size(); i++)
	{
		//LEAVE
		if (this->_Rooms[i].name == name_)
		{
			for (size_t j = 0; j < this->_Rooms[i].members.size(); j++)
			{
				if (this->_Rooms[i].members[j] == static_cast<unsigned int>(id))
				{
					std::vector<unsigned int>::iterator it = this->_Rooms[i].members.begin();
					for (size_t k = 0; k < j; k++)
						it++;
					for (size_t h = 0; h < this->_Rooms[i].members.size(); h++)
						mySend(replyPART(name_, the), this->_Rooms[i].members[h]);
					this->_Rooms[i].members.erase(it);
					if (this->_Rooms[i].members.size() < 1)
					{
						std::vector<Channel>::iterator it2 = this->_Rooms.begin();
						for (size_t k = 0; k < i; k++)
							it2++;
						this->_Rooms.erase(it2);
					}
					return 1;
				}
			}
			return 1;
		}
	}
	return 1;
}

void	Server::channelMess(std::string message, std::string channel, int ip){
	Find_USER
	for (size_t i = 0; i < this->_Rooms.size(); i++)
	{
		// SEND MESSAGE
		if (this->_Rooms[i].name == channel){
			this->_Rooms[i].sendAll(message, ip, the);
			return ;
		}
	}
	// NO CHANNEL FOUND
}

int	Server::CreateServer()
{
	int	tempFd = 0;
	int		on = 1;
	tempFd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (tempFd == -1)
	{
		std::cout << "\033[31m" << "error : during creation" << "\033[0" << std::endl;
		return -2;
	}
	if (setsockopt(tempFd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) == -1)
	{
		std::cout << "\033[31m" << "error : during settings" << "\033[0" << std::endl;
		return -1;
	}
	_socketFd = tempFd;
	std::memset(&_addr, 0, sizeof(_addr));
	_addr.sin_family = AF_INET;
	_addr.sin_addr.s_addr = INADDR_ANY;
	_addr.sin_port = htons(_port);
	if (bind(_socketFd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1)
	{
		std::cout << "\033[31m" << "error : during binding IP" << "\033[0" << std::endl;
		return -1;
	}
	if (listen(_socketFd, 3) == -1)
	{
		std::cout << "\033[31m" << "error : during starting to listen" << "\033[0" << std::endl;
		return -1;
	}
	struct pollfd tmp;
	tmp.fd = tempFd;
	tmp.events = POLLIN;
	_fds.push_back(tmp);
	//std::cout << _fds[0].fd << std::endl;
	_timeout = (1000 * 60 * TIMEOUT_M + TIMEOUT_S * 1000);
	fcntl(this->_socketFd, 4, O_NONBLOCK);
	return (1);
}

int	Server::StartServer(){
	this->_ON = true;
	do
	{
		int	rp = poll(&this->_fds[0], this->_fds.size(), this->_timeout);
		if (rp < 0){
			std::cout << "Error: poll failed" << std::endl;
			break ;
		}
		else if (rp == 0){
			std::cout << "Poll: timeout !" << std::endl;
			break ;
		}
		size_t	current_size = this->_fds.size();
		for (size_t i = 0; i < current_size; i++)
		{
			if (this->_fds[i].revents == 0)
				continue;
			if (this->_fds[i].revents == 17)
				std::cout << "user disconnect" << std::endl;
			else if (this->_fds[i].revents != POLLIN){
				std::cout << "Error ! revents = " << this->_fds[i].revents << std::endl;
				this->_ON = false;
				break ;
			}
			if (this->_fds[i].fd == this->_socketFd)
			{
				std::cout << "Listening socket is readable" << std::endl;
				int new_sd = -1;
				do
				{
					new_sd = accept(this->_socketFd, NULL, NULL);
					if (new_sd < 0){
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Error: accept failed" << std::endl;
							this->_ON = false;
						}
						break ;
					}
					std::cout << "New incoming connection : " << new_sd << std::endl;
					struct pollfd	newpollfd;
					newpollfd.fd = new_sd;
					newpollfd.events = POLLIN;
					this->_fds.push_back(newpollfd);
					User	user(newpollfd.fd);
					this->_Users.push_back(user);
				} while (new_sd != -1);
			}
			else
			{
				std::cout << "Descriptor is readable : " << this->_fds[i].fd << std::endl;
				int	close_conn = false;
				char	buffer[2000];
				int	rc = -1;
				int	len = -1;
				std::string	stay;
				do
				{
					rc = recv(this->_fds[i].fd, buffer, sizeof(buffer), 0);
					if (rc < 0)
					{
						if (errno != EWOULDBLOCK)
						{
							std::cout << "Error: recv failed : " << rc << std::endl;
							close_conn = true;
						}
						break ;
					}
					if (rc == 0)
					{
						int	ip = this->_fds[i].fd;
						Find_USER
						std::cout << "Connection closed" << std::endl;
						// USER_DISCONNECT
						for (size_t j = 0; j < this->_Rooms.size(); j++)
						{
							for (unsigned int ui = 0; ui < this->_Rooms[j].members.size(); ui++)
							{
								if (this->_Rooms[j].members[ui] == static_cast<unsigned int>(ip))
								{
									std::string part = "PART ";
									part += this->_Rooms[j].name + " :Weechat 3.7.1\r\n";
									std::cout << "MESS=" << part << std::endl;
									the->PART(part, *this);
								}
							}
						}
						this->_Users.erase(the);
						close_conn = true;
						break ;
					}
					len = rc;
					std::cout << len << " bytes received" <<std::endl;
					// std::string nul = buffer;
					// bool find_endline = false;
					// for (size_t k = 0; k < nul.size(); k++){
					// 	if (nul[k] == '\n'){
					// 		find_endline = true;
					// 		break ;
					// 	}
					// }
					// stay += buffer;
					// if (find_endline == true)
					// {
					// 	std::cout << "is backlashn" << std::endl;
					// 	for (size_t k2 = 0; k2 < stay.size(); k2++){
					// 		if (stay[k2] == '\n'){
					// 			stay.insert(k2, "\r");
					// 			k2++;
					// 		}
					// 	}
					// 	char*	tbuf = const_cast<char *>(stay.c_str());
						rc = this->action(this->_fds[i].fd, buffer, len);
						if (rc < 0)
						{
							if (rc == 0)
								std::cout << "Error: action failed" << std::endl;
							close_conn = true;
							break ;
						}
					// 	stay.clear();
					// }
				} while (true);
				if (close_conn)
				{
					close(this->_fds[i].fd);
					std::vector<struct pollfd>::iterator	it = this->_fds.begin();
					for (size_t z = 0; z < i; z++)
						it++;
					this->_fds.erase(it);
				}
			}
		}
	} while (this->_ON);
	for (size_t i = 0; i < this->_fds.size(); i++)
		close(this->_fds[i].fd);
	return 1;
}


int			Server::action(int ip, char* buf, int len)
{
	Find_USER
	std::string buffer = buf;
	if (the == this->_Users.end())
	{
		std::cout << "Error : no User found" << std::endl;
		return 0;
	}
	/*######## cut and print server side #########*/
	// if (true)
	// {
	// 	std::cout << buffer;
	// 	return 1;
	// } 
	/*--------------------------------------------*/
	std::vector<std::string> tab;
	int	last = 0;
	for (int i = 0; i < len; i++)
	{
		if (buf[i] == '\n'){
			std::string t = buffer.substr(last, i - last + 1);
			tab.push_back(t);
			last = i + 1;
		}
	}
	while (tab.size() > 0)
	{
		if (the->isConnected())
		{
			if (strFind("PASS ", tab[0])){
				mySend(ERR_ALREADYREGISTRED(), the->getID());
			}
			else if (strFind("USER ", tab[0])){
				mySend(ERR_ALREADYREGISTRED(), the->getID());
			}
			else if (strFind("JOIN ", tab[0])){
				std::cout << LINE << std::endl;
				the->JOIN(LINE, *this);
			}
			else if (strFind("PART ", tab[0])){
				std::cout << LINE << std::endl;
				the->PART(LINE, *this);
			}
			else if (strFind("PRIVMSG ", tab[0])){
				std::cout << LINE << std::endl;
				the->PRIVMSG(LINE, *this, ip);
			}
			else
				std::cout << LINE << std::endl;
		}
		else
		{
			if (strFind("NICK ", tab[0])){
				the->NICK(LINE, this->_Users);
			}
			else if (strFind("USER ", tab[0])){
				the->USER(LINE);
			}
			else if (strFind("PASS ", tab[0])){
				the->PASS(LINE);
			}

			// CONNECTION
			if (tab.size() == 1 && verif_connect(the, this->_password) == 1)
			{
				//if a hub
			}
		}
		// Erase the line we use (always use the line 0 first)
		tab.erase(tab.begin());
	}
	return 1;
}

int	verif_connect(std::vector<User>::iterator& the, std::string serv_pass){
	if (the->getMdp() != serv_pass && serv_pass.size() > 0)
		return 0;
	if (the->getUsername() == USER_DEFAULT_VALUE_STR)
		return 0;
	the->setConnect(true);
	std::string rep = replyWelcome(the);
	mySend(rep, the->getID());
	return 1;
}