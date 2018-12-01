#include "Fonts.h"
Fonts::Fonts(SDL_Renderer* rend):gRenderer(rend)
{
	if(!init())
		std::cout << "Text could not initialize!" << std::endl;
	if(!load_files())
		std::cout << "Text could not be loaded!" << std::endl;
	colorText = {255, 255, 255};
}
bool Fonts::init()
{
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1)
	{
		return false;
	}
	if(TTF_Init() == -1)
	{
		return false;
	}
	//SDL_WM_SetCaption("TTF Test", NULL );
	return true;
}
bool Fonts::load_files()
{
	font = TTF_OpenFont("resources/Fonts/Quivira.otf",28);
	if(font == NULL)
	{
		return false;
	}
	return true;
}
bool Fonts::RenderFont(std::string words)
{
	int textW;
	int textH;
	const char *wordsChar = words.c_str();
	message =  TTF_RenderText_Solid(font,wordsChar,colorText);
	if(message == NULL)
		return false;
	text = SDL_CreateTextureFromSurface(gRenderer, message);
	SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
	SDL_Rect messageBox = { 1280/2, 0, textW, textH };
	SDL_Rect imBox = { 0, 0, textW, textH };
	SDL_RenderCopy(gRenderer, text, &imBox, &messageBox);

}
void Fonts::CloseFont()
{
	SDL_DestroyTexture(text);
	SDL_FreeSurface(message);
	TTF_CloseFont(font);
	TTF_Quit();
}