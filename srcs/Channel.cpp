#include "../includes/Channel.hpp"
#include "../includes/irc.hpp"

int	mySend(std::string mess, int ip);

int	Channel::sendAll(std::string message, int ip, std::vector<User>::iterator& the){
	//std::cout << message << std::endl;
	for (size_t i = 0; i < this->members.size(); i++){
		if (this->members[i] != static_cast<unsigned int>(ip)){
			mySend(replyMess(message, the, this->name), this->members[i]);
		}
	}
	std::cout << "RPL=" << replyMess(message, the, this->name) << std::endl;
	return 1;
}