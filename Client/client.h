#pragma once

#include <string>
#include <unistd.h>
#include <iostream>

using std::string;

class Client
{
	string name_;
	unsigned int port_;
	unsigned int period_;

	int sock;
	static bool isConnected_;
public:
	Client(string name, unsigned port, unsigned period) : name_(name), port_(port), period_(period) {}
	Client(const Client& c) : name_(c.name_), port_(c.port_), period_(c.period_), sock(c.sock) {}
	Client(Client&& c) noexcept { swap(c); }

	void connect();
	void send();

	void swap(Client& c) noexcept;

	string getName() const { return name_; }

	unsigned getPort()	const noexcept { return port_; }

	unsigned getPeriod() const noexcept { return period_; }

	Client& operator=(const Client& c)
	{
		Client copy(c);
		swap(copy);
		return *this;
	}

	Client& operator=(Client&& c) noexcept
	{
		swap(c);
		return *this;
	}

	~Client() { close(sock); }

private:
	string generateMessage();
	static void disconnect() 
	{
		std::cout << "Press any key to disconnect\n";
		getchar();
		isConnected_ = false; 
	}
};
