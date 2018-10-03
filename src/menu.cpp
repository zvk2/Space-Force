//general outline for button events on menu and how to deal with them
//still editing to get it to work

#ifdef __APPLE__
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#else
#include <SDL.h>
#include <SDL_image.h>
#endif
#include <string>

//Screen attributes
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;
const int SCREEN_BPP = 32;

//The button states in the sprite sheet
const int CLIP_START = 0;
const int CLIP_SCORES = 1;
const int CLIP_EXIT = 2;
const int CLIP_LEVELS = 3;

//The surfaces
SDL_Surface *buttonSheet = NULL;
SDL_Surface *screen = NULL;

//event structure
SDL_Event event;

//gets clips from sprite sheet
SDL_Rect clips[4];

class button
{
    private:

    SDL_Rect box;
    SDL_Rect* clip;

    public:

    Button( int x, int y, int w, int h );
    void handle_events();
    void show();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    loadedImage = IMG_Load( filename.c_str() );

    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //error
    if( screen == NULL )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Menu", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    //Load the button sprite sheet
    buttonSheet = load_image( "resources/imgs/main_button_sheet.png" );

    //If there was a problem in loading the button sprite sheet
    if( buttonSheet == NULL )
    {
        return false;
    }

    //If everything loaded fine
    return true;
}

void clean_up()
{
    SDL_FreeSurface( buttonSheet );
    SDL_Quit();
}

void set_clips()
{
    //Clip the sprite sheet
    clips[ CLIP_START ].x = 0;
    clips[ CLIP_START ].y = 0;
    clips[ CLIP_START ].w = 640;
    clips[ CLIP_START ].h = 360;

    clips[ CLIP_SCORES ].x = 640;
    clips[ CLIP_SCORES ].y = 0;
    clips[ CLIP_SCORES ].w = 640;
    clips[ CLIP_SCORES ].h = 360;

    clips[ CLIP_EXIT ].x = 0;
    clips[ CLIP_EXIT ].y = 360;
    clips[ CLIP_EXIT ].w = 640;
    clips[ CLIP_EXIT ].h = 360;

    clips[ CLIP_LEVELS ].x = 640;
    clips[ CLIP_LEVELS ].y = 360;
    clips[ CLIP_LEVELS ].w = 640;
    clips[ CLIP_LEVELS ].h = 360;
}

Button::Button( int x, int y, int w, int h )
{
    box.x = x;
    box.y = y;
    box.w = w;
    box.h = h;

    //default sprite
    clip = &clips[ CLIP_MOUSEOUT ];
}

void Button::handle_events()
{
    //mouse offsets
    int x = 0, y = 0;

    //If the mouse moved
    if( event.type == SDL_MOUSEMOTION )
    {
        //mouse offsets
        x = event.motion.x;
        y = event.motion.y;

        //mouse is over the button
        if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
        {
            //button sprite
            clip = &clips[ CLIP_MOUSEOVER ];
        }
        else
        {
            //button sprite
            clip = &clips[ CLIP_MOUSEOUT ];
        }
    }
    //mouse button was pressed
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        //left mouse button was pressed
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_MOUSEDOWN ];
            }
        }
    }
    //mouse button was released
    if( event.type == SDL_MOUSEBUTTONUP )
    {
        //If the left mouse button was released
        if( event.button.button == SDL_BUTTON_LEFT )
        {
            //Get the mouse offsets
            x = event.button.x;
            y = event.button.y;

            //mouse is over the button
            if( ( x > box.x ) && ( x < box.x + box.w ) && ( y > box.y ) && ( y < box.y + box.h ) )
            {
                //Set the button sprite
                clip = &clips[ CLIP_MOUSEUP ];
            }
        }
    }
}

void Button::show()
{
    //show button
    apply_surface( box.x, box.y, buttonSheet, screen, clip );
}

int run_menu( int argc, char* args[] )
{
    bool quit = false;

    //start
    if( init() == false )
    {
        return 1;
    }

    if( load_files() == false )
    {
        return 1;
    }

    //clip the sprite sheet
    set_clips();

    //make the button
    Button myButton( 170, 120, 320, 240 );

    while( quit == false )
    {
        //handle event
        if( SDL_PollEvent( &event ) )
        {
        
            myButton.handle_events();

            //closed window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }

        //fill the screen white
        SDL_FillRect( screen, &screen->clip_rect, SDL_MapRGB( screen->format, 0xFF, 0xFF, 0xFF ) );

        //show the button
        mButton.show();

        //update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }
    }

    clean_up();

    return 0;
}