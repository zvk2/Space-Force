
#include <iostream>
#include <string>
#include "INC_SDL.h"
#include "ClientSocket.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

class ClientInterface
{
	private:
		// Create a pointer to a ClientSocket object
		ClientSocket *cs;
	
	public:
		
		ClientInterface();

		~ClientInterface();

		int Connect();

		SDL_Rect Communicate(SDL_Rect playerRect);

		string Serialize(SDL_Rect playerRect);
		
		SDL_Rect GetPlayerInfo(string playerInfo);
};
