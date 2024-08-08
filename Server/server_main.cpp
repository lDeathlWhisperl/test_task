#include "server.h"

#include <string>
#include <iostream>

int main(int argc, char* argv[])
{
	system("clear");
	unsigned port;
	try
	{
		port = std::stoi(std::string(argv[1]));
	}
	catch(...)
	{
		std::cerr << "Invalid argument\n";
		return 1;
	}

	Server server(port);
	
	server.bootup();

	std::cout << "Press any key to shutdown\n";
	getchar();
	std::cout << "Good bye!\n";

	server.shutdown();
	return 0;
}
