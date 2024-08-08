#!/bin/bash

g++ -std=c++20 -c Server/server_main.cpp
g++ -std=c++20 -c Server/server.cpp

g++ -std=c++20 -c Client/client_main.cpp
g++ -std=c++20 -c Client/client.cpp

g++ server_main.o server.o -g -o server
g++ client_main.o client.o -o client

rm *.o
