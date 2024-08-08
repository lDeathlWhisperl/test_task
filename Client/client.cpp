#include "client.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

bool Client::isConnected_;

void Client::connect()
{
	if(isConnected_) return;

    sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	std::cout << "Connecting" << std::flush;
    while(::connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
		std::cout << '.' << std::flush;
		sleep(2);
    }
	std::cout << '\n';
	isConnected_ = true;

	std::thread(disconnect).detach();
}

string Client::generateMessage()
{
	using namespace std::chrono;
	auto timestamp = system_clock::now().time_since_epoch();

	string year_    	 = std::to_string(1970 + duration_cast<years>(timestamp).count());
	string month_   	 = std::to_string(1 + duration_cast<months>(timestamp).count() % 12);
	string day_          = std::to_string(1 + duration_cast<days>(timestamp).count() % 7);
	string hours_   	 = std::to_string(3 + duration_cast<hours>(timestamp).count() % 24);
	string minutes_ 	 = std::to_string(duration_cast<minutes>(timestamp).count() % 60);
	string seconds_      = std::to_string(duration_cast<seconds>(timestamp).count() % 60);
	string milliseconds_ = std::to_string(duration_cast<milliseconds>(timestamp).count()%1000);
	
	return "[" + year_ + "-" + month_ + "-" + day_ + " " + 
		hours_ + ":" + minutes_ + ":" + seconds_ + "." + milliseconds_ + "] " + name_;
}

void Client::send()
{
	while(isConnected_)
	{
		string message = generateMessage();
		if(::send(sock, message.c_str(), sizeof(message), MSG_NOSIGNAL) == -1) break;
		sleep(period_);
	}
}

void Client::swap(Client& c) noexcept
{
	std::swap(name_, c.name_);
	std::swap(port_, c.port_);
	std::swap(period_, c.period_);
	std::swap(sock, c.sock);
}
