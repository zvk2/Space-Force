#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <sstream>
#include "SDL_net.h"


using std::string;
using std::cout;
using std::cerr;
using std::endl;

class ServerSocket
{
	private:
		bool debug;

		unsigned int port;
		unsigned int bufferSize;

		IPaddress serverIP;
		TCPsocket serverSocket;
		string    dotQuadString;
		TCPsocket *clientList;
		bool *socketFree;
		char *dataBuffer;

		SDLNet_SocketSet socketSet;

		unsigned int clientCount;

		bool shutdownServer;

	public:
		static const string ACCEPTED;
		static const string DENIED;

		ServerSocket(unsigned int port, unsigned int bufferSize, unsigned int maxSockets);

		~ServerSocket();

		void CheckForConnections();

		int CheckForActivity();

		void DealWithActivity(unsigned int clientNumber);

		bool GetShutdownStatus();
};

// Template function to convert anything to a string
template<class T>
std::string ToString(const T& t)
{
	std::ostringstream stream;
	stream << t;
	return stream.str();
}

#endif
