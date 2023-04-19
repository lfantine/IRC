#include "../includes/irc.hpp"

int	pars_port(char* port)
{
	std::string	p;
	p += port;
	for (size_t i = 0; i < p.length(); i++){
		if (!std::isdigit(p[i]))
		{
			std::cout << "error: '" << p[i] << "' in " + p << " is not a number" << std::endl;
			return (0);
		}
	}
	if (p.length() < 1)
	{
		std::cout << "error: why do you try this ?" << std::endl;
		return (0);
	}
	if (p.length() > 5)
	{
		std::cout << "error: port too long" << std::endl;
		return (0);
	}
	int	nb = std::stoi(p);
	if (nb > MAXPORT)
	{
		std::cout << "error: port too big. (1 <= port <= " << MAXPORT << ")" << std::endl;
		return (0);
	}
	// changement
	return (1);
}

int	pars_password(char* pass)
{
	std::string p;
	p += pass;
	for (size_t i = 0; i < p.length(); i++){
		if (!std::isalnum(p[i]))
		{
			std::cout << "error: '" << p << "' is not a valid password" << std::endl;
			return (0);
		}
	}
	if (p.length() < 1)
	{
		std::cout << "\033[31mError:\033[0m why do you try this ?" << std::endl;
		return (0);
	}
	return (1);
}

int	pars_hub(int ac, char **av)
{
	if (ac != 3)
	{
		std::cout << "Usage : ./ircserv <port> <password>" << std::endl;
		return (0);
	}
	if (!pars_port(av[1]))
		return 0;
	if (!pars_password(av[2]))
		return 0;
	return 1;
}

int	main(int ac, char **av)
{
	if (!pars_hub(ac, av))
		return (1);
	//t_system	sys;
	Server		server(av[1], av[2]);
	int	tmp6 = server.CreateServer();
	if (tmp6 < 0)
	{
		if (tmp6 == -1)
			close(server.getServerFD());
		return -1;
	}
	server.StartServer();
	close(server.getServerFD());
	return (0);
}