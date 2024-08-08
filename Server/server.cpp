#include "server.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <strings.h>
#include <fcntl.h>

std::ofstream Server::fout;
std::vector<std::thread> Server::reciever;
bool Server::isConnectable_ = false;
int Server::sock;

Server::Server(unsigned port) : port_(port)
{
	fout.open("log.txt");
	sockaddr_in addr;

	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
		perror("socket");
		exit(1);
	}
	fcntl(sock, F_SETFL, O_NONBLOCK);

	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port_);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	int optval = 1;
	if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0)
	{
		perror("setsockopt");
		exit(2);
	}

	if(bind(sock, (sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		exit(3);
	}

	if(::listen(sock, 0) < 0)
	{
		perror("listen");
		exit(4);
	}
}

void Server::swap(Server& s) noexcept
{
	std::swap(port_, s.port_);
	std::swap(sock,  s.sock);
}

void Server::bootup()
{
	if(isConnectable_) return;
	isConnectable_ = true;

	std::thread(listen).detach();
}

void Server::listen()
{
	while(isConnectable_)
	{
		int client = accept(sock, nullptr, nullptr);
		if(client == -1) continue;
		
		reciever.push_back(std::thread(recieve, client));
	}
}

void Server::recieve(const int client)
{
	char message[80];

	while(true && isConnectable_)
	{
		switch(recv(client, &message, 80, 0))
		{
		case -1:
			perror("recv");
			goto stop;
			break;
		case 0:
			if(::shutdown(client, SHUT_RDWR) == -1) perror("shutdown");
			goto stop;
			break;
		default:
			fout << message << '\n' << std::flush;
			break;
		}
	}
	stop:
	close(client);
}
