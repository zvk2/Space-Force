#include <iostream>
#include <string>
#include "SDL_net.h"
#include "ClientInterface.h"
#include "ClientSocket.h"


ClientSocket *cs;

ClientInterface::ClientInterface()
{
	// Initialise SDL net
	if (SDLNet_Init() == -1)
	{
		std::cerr << "Failed to intialise SDL_net: " << SDLNet_GetError() << std::endl;
		exit(-1);
	}

	try
	{
		// Initialize client socket with server address, port number, max message size
		cs = new ClientSocket("127.0.0.1", 1234, 512);
	}
	catch (SocketException e)
	{
		std::cerr << e.what()   << std::endl;
	}
}

ClientInterface::~ClientInterface()
{
	// Shutdown SDLNet
	SDLNet_Quit();
	delete cs;
}

int ClientInterface::Connect(){
	int connected = 0;
	try
	{
		// Connect to the server at the specified IP & port
		connected = cs->ConnectToServer();
	}
	catch (SocketException e)
	{
		cerr << e.what() << endl;
	}
	return connected;
}

SDL_Rect ClientInterface::Communicate(SDL_Rect playerRect){

	string receivedMessage = "";

	try
	{
		// Serialize the message with our personal protocol
		string transmitInfo = Serialize(playerRect);
		// Transmit that data
		cs->Transmit(transmitInfo);
		// Check for incoming messages
		receivedMessage = cs->CheckForIncomingMessages();
		// Deserialize received data and return it
		if (receivedMessage != "")
		{
			return GetPlayerInfo(receivedMessage);	
		}
	}
	catch (SocketException e)
	{
		cerr << e.what() << endl;
	}
}

// Current serialization method, this will change as the game changes
string ClientInterface::Serialize(SDL_Rect playerRect)
{
	// Get the x and y values and standardize them to be a string of length 4
	int x = playerRect.x;
	int y = playerRect.y;
	string xS = "";
	string yS = "";
	string stringified = "";
	if(x < 10)
	{
		xS = "000" + std::to_string(x);
	}
	else if(x < 100)
	{
		xS = "00" + std::to_string(x);
	}
	else if(x < 1000)
	{
		xS = "0" + std::to_string(x);
	}
	else
	{
		xS = std::to_string(x);
	}

	if(y < 10)
	{
		yS = "000" + std::to_string(y);
	}
	else if(y < 100)
	{
		yS = "00" + std::to_string(y);
	}
	else if(y < 1000)
	{
		yS = "0" + std::to_string(y);
	}
	else
	{
		yS = std::to_string(y);
	}

	// Return serialized value	
	stringified = xS + yS;
	return stringified;
}

// Deserialization method, as the serialization method changes, so to will this
SDL_Rect ClientInterface::GetPlayerInfo(string playerInfo)
{
	SDL_Rect playerRect = {0, 0, 0, 0};
	playerRect.x = std::stoi(playerInfo.substr(0, 4));
	playerRect.y = std::stoi(playerInfo.substr(5, 4));

	return playerRect;
}
