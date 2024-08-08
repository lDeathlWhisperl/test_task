#include "client.h"
#include <iostream>

int main(int argc, char *argv[])
{
	unsigned port, period;
	try
	{
		port   = std::stoi(string(argv[2]));
		period = std::stoi(string(argv[3]));
	}
	catch(...)
	{
		std::cerr << "Invalid arguments\n";
		return 1;
	}

	Client client(argv[1], port, period);
	
	client.connect();
	client.send();
	return 0;
}
