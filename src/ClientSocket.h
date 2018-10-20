#ifndef SOCKET_H
#define SOCKET_H

#include <string>
#include <sstream>
#include "SDL_net.h"
#include "SocketException.h"

using std::string;

class ClientSocket
{
	private:

		SDLNet_SocketSet socketSet;
		// The server
		TCPsocket serverSocket;
		// Us and our communication channel
		TCPsocket clientSocket;
		// The port of the server to connect to
		unsigned int serverPort;
		// Our transmitted data size
		unsigned int dataSize;
		// The host name of the server (i.e. "localhost")
		string hostName;
		// IP address of the server, no periods
		IPaddress serverAddress;
		// String version of IP with dots
		string IP;
		// character array that stores received and transmitted data
		char *dataBuffer;

	public:
		static const string       SERVER_NOT_FULL;
		static const unsigned int MAX_CLIENTS;
		static const unsigned int TIMEOUT_PERIOD;
		static const unsigned int SOCKET_SET_POLL_PERIOD;

		ClientSocket(string theServerAddress, unsigned int theServerPort, unsigned int theBufferSize);

		~ClientSocket();

		// Function to poll for clients connecting
		int ConnectToServer();

		// Function to check the server for incoming messages
		string CheckForIncomingMessages();

		// Function to get keypresses in a non-blocking manner
		void Transmit(string playerInfo);
};

// Template function to convert most anything to a string
template<class T>
std::string ToString(const T& t)
{
	std::ostringstream stream;
	stream << t;
	return stream.str();
}


#endif
