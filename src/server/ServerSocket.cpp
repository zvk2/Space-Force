#include "ServerSocket.h"
#include <string.h>
#include <stdio.h>

const string ServerSocket::ACCEPTED = "OK";
const string ServerSocket::DENIED     = "FULL";
const string SHUTDOWN_SIGNAL = "/shutdown";     
const int maxSockets = 3;               
const int maxClients = 2;

ServerSocket::ServerSocket(unsigned int thePort, unsigned int theBufferSize, unsigned int theMaxSockets)
{
	port       = thePort;
	bufferSize = theBufferSize;

	clientList = new TCPsocket[maxClients];
	socketFree = new bool[maxClients];
	dataBuffer = new char[bufferSize];

	clientCount = 0;

	socketSet = SDLNet_AllocSocketSet(maxSockets);
	if (socketSet == NULL)
	{
		string msg = "Failed to allocate the socket set: ";
		msg += SDLNet_GetError();
	}

	for (unsigned int loop = 0; loop < maxClients; loop++)
	{
		clientList[loop] = NULL;
		socketFree[loop] = true;
	}

	int hostResolved = SDLNet_ResolveHost(&serverIP, NULL, port);

	if (hostResolved == -1)
	{
		string msg = "Failed to open the server socket: ";
		msg += SDLNet_GetError();
	}

	serverSocket = SDLNet_TCP_Open(&serverIP);

	if (!serverSocket)
	{
		string msg = "Failed to open the server socket: ";
		msg += SDLNet_GetError();
	}

	SDLNet_TCP_AddSocket(socketSet, serverSocket);
}

// ServerSocket destructor
ServerSocket::~ServerSocket()
{
	for (unsigned int loop = 0; loop < maxClients; loop++)
	{
		if (socketFree[loop] == false)
		{
			SDLNet_TCP_Close(clientList[loop]);
			socketFree[loop] = true;
		}
	}

	SDLNet_TCP_Close(serverSocket);

	SDLNet_FreeSocketSet(socketSet);

	delete clientList;
	delete socketFree;
	delete dataBuffer;
}


void ServerSocket::CheckForConnections()
{
	// Check every 1 ms, DO NOT SET TO 0
	int numActiveSockets = SDLNet_CheckSockets(socketSet, 1);
	// Check if someone is trying to connect, returns non-zero if there is
	int serverSocketActivity = SDLNet_SocketReady(serverSocket);

	if (serverSocketActivity != 0)
	{
		// If available space
		if (clientCount < maxClients)
		{
			for (unsigned int free = 0; free < maxClients; free++)
			{
				// Found a spot for socket to connect
				if (socketFree[free] == true)
				{
					socketFree[free] = false;

					// Accept connection and store it in the client list at free location
					clientList[free] = SDLNet_TCP_Accept(serverSocket);
					// Add the socket to our connection
					SDLNet_TCP_AddSocket(socketSet, clientList[free]);
					// Increment client count to keep track of how many connections
					clientCount++;
					// Tell the client that they've been accepted
					strcpy( dataBuffer, ACCEPTED.c_str() );
					int msgLength = strlen(dataBuffer) + 1;
					SDLNet_TCP_Send(clientList[free], (void *)dataBuffer, msgLength);
					printf("New client connection at %d\n", free);
					break;
				}
			}
		}
		else
		{
			// Tell the client they can't connect so they're not timed out
			TCPsocket tempSock = SDLNet_TCP_Accept(serverSocket);
			strcpy(dataBuffer, DENIED.c_str());
			int msgLength = strlen(dataBuffer) + 1;
			SDLNet_TCP_Send(tempSock, (void *)dataBuffer, msgLength);
			SDLNet_TCP_Close(tempSock);
		}

	}

}

void ServerSocket::DealWithActivity(unsigned int clientNumber)
{
	// read data buffer into a string
	string bufferContents = dataBuffer;
	// go over all clients
	for (unsigned int loop = 0; loop < maxClients; loop++)
	{
		// get message length
		unsigned int msgLength = strlen(dataBuffer) + 1;
		// if there is a message the send it to the client that didnt send it
		if ( (msgLength > 1) && (loop != clientNumber) && (socketFree[loop] == false) )
		{
			// send
			SDLNet_TCP_Send(clientList[loop], (void *)dataBuffer, msgLength);
		}
	}
	// check if user told server to shutdown
	if ( bufferContents.compare(SHUTDOWN_SIGNAL) == 0 )
	{
		shutdownServer = true;
	}

}

bool ServerSocket::GetShutdownStatus(){
	return shutdownServer;
}

int ServerSocket::CheckForActivity()
{
	// go over all clients
	for (unsigned int client = 0; client < maxClients; client++)
	{
		// check if they're sending data
		int activity = SDLNet_SocketReady(clientList[client]);

		if (activity != 0)
		{
			// receive sent data
			int receivedByteCount = SDLNet_TCP_Recv(clientList[client], dataBuffer, bufferSize);

			if (receivedByteCount <= 0)
			{
				// close sockets that aren't sending data anymore
				SDLNet_TCP_DelSocket(socketSet, clientList[client]);
				SDLNet_TCP_Close(clientList[client]);
				clientList[client] = NULL;

				socketFree[client] = true;

				clientCount--;

			}
			else
			{
				return client;
			}

		} 

	}
	return -1;
}
