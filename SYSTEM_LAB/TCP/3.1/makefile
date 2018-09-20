all: client server

CXX = g++
	
client : Client.cpp Utility.h
	$(CXX) Client.cpp -lpthread -o client

server : Server.cpp Utility.h
	$(CXX) Server.cpp -o server
