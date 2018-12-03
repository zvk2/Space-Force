#include "server/ServerSocket.h"
#include <string.h>
#include <stdio.h>

typedef struct NetworkData{
	int numPlayers;

	SDL_Rect Players[2];
	bool attack;

} NetworkData;

string NetworkItem::SerializeData(NetworkData);
NetworkData NetworkItem::DeSerializeData(string);
string NetworkItem::SerializeItem(string);


//order:
	//players
		//x
		//y
		//hitpoints
	//Attack?
		//if attack boolean is true then we know we've sent an attack, use the current x, y player position to start the attack from the client side

string NetworkItem::SerializeData(NetworkData d){
string data = "";
int i;
strcat(data, SerializeItem(std::to_string(numPlayers)));
strcat(data, SerializeItem(std::to_string(numAttacks)));

for(i = 0; i < numPlayers; i++){
int x = d.Players[i].x;
strcat(data, SerializeItem(std::to_string(x)));
x = d.Players[i].y; 
strcat(data, SerializeItem(std::to_string(x))); 
x = d.Players[i].hitpoints;
strcat(data, SerializeItem(std::to_string(x))); 
}

if(attack){
strcat(data, "0001");
}else{
strcat(data, "0000");
}

return data;
}

NetworkData NetworkItem::DeSerializeData(string d){
	NetworkData item;
	int readItems = 0;

	numPlayers = d.substr(readItems*4, 4);
	readItems++;
	numAttacks = d.substr(readItems*4, 4);
	readItems++;

	for(int i = 0; i < numPlayers; i++){
		item.Players[i].x = substr(readItems*4, 4);
		readItems++;
		item.Players[i].y = substr(readItems*4, 4);
		readItems++;
		item.Players[i].hitpoints = substr(readItems*4, 4);
		readItems++;
	}

	if(substr(readItems*4, 4) == "0000"){
		item.attack = false;
	}else{
		item.attack = true;
	}

	return item;
}

string NetworkItem::SerializeItem(string d){
	string data = "";

	if(d < 10)
	{
		data = "000" + std::to_string(d);
	}
	else if(d < 100)
	{
		data = "00" + std::to_string(d);
	}
	else if(d < 1000)
	{
		data = "0" + std::to_string(d);
	}
	else
	{
		data = std::to_string(d);
	}

	return data;
}
