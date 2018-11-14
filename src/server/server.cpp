// Library dependencies: libSDL, libSDL_net

#include <iostream>
#include <string.h>
#include <stdio.h>
#include "SDL_net.h"
#include "ServerSocket.h"

int main(int argc, char *argv[])
{

	// Initialise SDL_net
	if (SDLNet_Init() == -1)
	{
		std::cerr << "Failed to intialise SDL_net: " << SDLNet_GetError() << std::endl;
		exit(-1);
	}

	// Create a pointer to a ServerSocket object
	ServerSocket *ss;

	try
	{
		// Not try to instantiate the server socket
		// Parameters: port number, buffer size (i.e. max message size), max sockets
		ss = new ServerSocket(1234, 512, 3);
	}
	catch (int e)
	{
		std::cerr<<"Server could not be created"<<endl;
	}

	try
	{
		// Specify which client is active, -1 means "no client is active"
		int activeClient = -1;

		// server runs here
		do
		{
			// Check for any incoming connections to the server socket
			ss->CheckForConnections();

			// while connected with clients go through data
			do
			{
				// see if data is being received
				activeClient = ss->CheckForActivity();

				// If there's a client with activity
				if (activeClient != -1)
				{
					// process the data
					ss->DealWithActivity(activeClient);
				}

			// When there are no more clients with activity to process, continue...
			} while (activeClient != -1);

		// ...until we've been asked to shut down.
		} while (ss->GetShutdownStatus() == false);

	}
	catch (int e)
	{
		std::cerr<<"Error running the server"<<endl;
	}

	// Shutdown SDLNet - our ServerSocket will clean up after itself on destruction
	SDLNet_Quit();

	return 0;
}
