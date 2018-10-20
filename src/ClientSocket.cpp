#include "ClientSocket.h"
#include <string.h>
#include <stdio.h>

const std::string  ClientSocket::SERVER_NOT_FULL = "OK";    
const unsigned int ClientSocket::MAX_CLIENTS = 2;
const unsigned int ClientSocket::TIMEOUT_PERIOD = 5000;        
const unsigned int ClientSocket::SOCKET_SET_POLL_PERIOD = 10;

// Constructor receives the IP, the port and the size of the transmit buffer
ClientSocket::ClientSocket(string serverAddress, unsigned int specifiedServerPort, unsigned int sizeOfData)
{
	// Set IP
	hostName = serverAddress;
	// Set port
	serverPort = specifiedServerPort;
	// Set data size
	dataSize = sizeOfData;

	// Doesnt throw errors yet, depends how we want to handle this
	try
	{
		// Initialize buffer
		dataBuffer = new char[dataSize];
		// Initialize socket set
		socketSet = SDLNet_AllocSocketSet(2);
		if (socketSet == NULL)
		{
			string msg = "Failed to allocate the socket set in ClientSocket constructor: ";
			msg += SDLNet_GetError();
		}
	}
	catch (SocketException e)
	{
	}
}

ClientSocket::~ClientSocket()
{
	// Clean up sockets, delete allocated memory
	SDLNet_TCP_Close(serverSocket);
	SDLNet_TCP_Close(clientSocket);

	SDLNet_FreeSocketSet(socketSet);

	delete dataBuffer;
}

int ClientSocket::ConnectToServer()
{
	// First resolve the hostname to an IP
	int hostResolved = SDLNet_ResolveHost(&serverAddress, hostName.c_str(), serverPort);

	if (hostResolved == -1)
	{
		string msg = "Error: Failed to resolve the server hostname to an IP address.";

		SocketException e(msg);
		throw e;
	}

	// Open a socket between server and client
	clientSocket = SDLNet_TCP_Open(&serverAddress);

	// If client socket is null we did not start communication
	if (!clientSocket)
	{
		string msg = "Error: Failed to open connection to server: ";
		msg += SDLNet_GetError();

		SocketException e(msg);
		throw e;
	}
	else 
	{
		// Add the socket to our socket set
		SDLNet_TCP_AddSocket(socketSet, clientSocket);
		
		// Must call check sockets before checking if data is waiting to be processed
		SDLNet_CheckSockets(socketSet, ClientSocket::TIMEOUT_PERIOD);

		// Socket ready returns non-zero if there is information being sent to us
		int gotServerResponse = SDLNet_SocketReady(clientSocket);

		if (gotServerResponse != 0)
		{
			/* 
			 * Get the transmitted data, if it's 0 then we lost connection 
			 * otherwise we're looking for connection confirmation
			 */
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, dataBuffer, dataSize);

			if ( dataBuffer == ClientSocket::SERVER_NOT_FULL )
			{
				// Connected
				return 1;
			}
		}
	}
	// Not connected
	return 0;
}

string ClientSocket::CheckForIncomingMessages()
{
	// Initialize message to empty
	string receivedMessage = "";

	// Must call this before calling SocketReady
	int activeSockets = SDLNet_CheckSockets(socketSet, ClientSocket::SOCKET_SET_POLL_PERIOD);

	if (activeSockets != 0)
	{
		// Check if there are any messages waiting to be processed, returns 1 if messages incoming
		int gotMessage = SDLNet_SocketReady(clientSocket);

		if (gotMessage != 0)
		{
			// Put the message in the data buffer and check that it's not empty before processing
			int serverResponseByteCount = SDLNet_TCP_Recv(clientSocket, dataBuffer, dataSize);

			if (serverResponseByteCount != 0)
			{
				// Convert message to string
				receivedMessage = toString(dataBuffer);
			}
			else
			{
				// Lost connection
			}

		}

	}
	// Return either empty string or message from server
	return receivedMessage;
}

void ClientSocket::Transmit(string dataToTransmit)
{
	// Place the data in the buffer
	strcpy( dataBuffer, dataToTransmit.c_str() );
	// Get the data size for transmission
	unsigned int inputLength = strlen(dataBuffer) + 1;

	//Send, along my socket, the data and its size
	if (SDLNet_TCP_Send(clientSocket, (void *)dataBuffer, inputLength) < inputLength)
	{
		string msg  = "Error: Failed to send message: ";
		       msg += SDLNet_GetError();
		SocketException e(msg);
		throw e;
	}
}