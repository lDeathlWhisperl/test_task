#pragma once

#include <unistd.h>
#include <iostream>
#include <vector>
#include <thread>
#include <fstream>

class Server
{
	static std::ofstream fout;
	unsigned int port_;
	static int sock;
	static bool isConnectable_;

	static std::vector<std::thread> reciever;
public:
	explicit Server(unsigned port);
	Server(const Server&) = delete;
	Server(Server&& s) noexcept { swap(s); }

	Server& operator=(const Server&) = delete;
	Server& operator=(Server&& s) 
	{ 
		swap(s); 
		return *this;
	}
	
	void bootup();
	
	void shutdown() noexcept { isConnectable_ = false; }


	~Server()
	{
		close(sock);
		shutdown();
		fout.close();
		
		while(!reciever.empty())
		{
			const auto& thread = std::find_if(reciever.begin(), reciever.end(), 
					[](std::thread &th){ return th.joinable(); });
			thread->join();
			reciever.erase(thread);
		}
	}
	
private:
	void swap(Server& s) noexcept;
	static void listen();
	static void recieve(const int);
};
