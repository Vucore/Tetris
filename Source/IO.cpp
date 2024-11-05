#include <iostream>
#include "IO.h"
#include "Board.h"
#define FONT "Font//Ducine.ttf"
#define Start "image//StartGame3D.jpg"
using namespace std;

IO::IO()
{
	_renderer = nullptr;
	_window = nullptr;
	InitalizeGraphics();
}

IO::~IO()
{
	SDL_DestroyRenderer(_renderer);
	SDL_DestroyWindow(_window);
	_renderer = nullptr;
	_window = nullptr;
	SDL_Quit();
}

void IO::InitalizeGraphics()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		cerr << " SDL Error: " << SDL_GetError() << endl;
		return;
	}
	if (TTF_Init() == -1)
	{
		cerr << "SDL_ttf could not initialize! TTF_Error! " << TTF_GetError() << endl;
		return;
	}
	
	int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;  // Mã hóa các định dạng bạn muốn hỗ trợ
    if ((IMG_Init(imgFlags) & imgFlags) != imgFlags) 
	{
        cerr << "IMG_Init Error: " << IMG_GetError() << endl;
        return;
    }
    
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {

        cerr << "SDL_mixer could not initialize! SDL_mixer Error: %s" << Mix_GetError();
        return;

    }
	_window = SDL_CreateWindow("Tetris in C++!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
	{
		cerr << " SDL Error: " << SDL_GetError() << endl;
		return;
	}
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
	if (_renderer == nullptr)
	{
		cerr << " SDL Error: " << SDL_GetError() << endl;
		return;
	}

}

void IO::SetRendererColor(const Color color)
{
    switch (color)
    {
        case RED:
            SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
            break;
        case BLUE:
            SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
            break;
        case AMBER:
            SDL_SetRenderDrawColor(_renderer, 255, 191, 240, 255);
            break;
        case GREEN:
            SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
            break;
        case CYAN:
            SDL_SetRenderDrawColor(_renderer, 0, 255, 255, 255);
            break;
        case MAGENTA:
            SDL_SetRenderDrawColor(_renderer, 255, 0, 255, 255);
            break;
        case ORANGE:
            SDL_SetRenderDrawColor(_renderer, 255, 165, 0, 255);
            break;
        case YELLOW:
            SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
            break;
		case GRAY:
		    SDL_SetRenderDrawColor(_renderer, 130, 130, 130, 255);
			break;
		case WHITE:
		    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
			break;
    }
}

void IO::ClearScreen()
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255); 
	SDL_RenderClear(_renderer);
}

void IO::UpdateScreen()
{
	SDL_RenderPresent(_renderer);
}

void IO::DrawRectangle(const int x, const int y, const int width, const int height, const Color color)
{
	SDL_Rect fillArea = { x, y, width, height };
	SetRendererColor(color);
	SDL_RenderFillRect(_renderer, &fillArea);
}

void IO::DrawRectangleOutline(const int x, const int y, const int width, const int height, const Color color)
{
	SDL_Rect outlineArea = { x, y, width, height };
	SetRendererColor(color);
	SDL_RenderDrawRect(_renderer, &outlineArea);
}

int IO::GetKeyPress()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		switch (e.type)
		{
		case SDL_KEYDOWN:
			return e.key.keysym.sym;
		case SDL_QUIT:
			exit(1);
		}
	}

	return -1;
}

void IO::WriteText(string text, const int t, const int x,const int y, SDL_Color color)
{
	// Load font
	TTF_Font* font = TTF_OpenFont(FONT, t);  
	if (font == nullptr)
	{
		cerr << "Failed to load font! TTF_Error! " << TTF_GetError() << endl;
		return;
	}
	// Tạo texture từ text
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color); // tạo bề mặt + nội dung hiển thị
	if (textSurface == nullptr)
	{
		cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << endl;
	}
	else
	{
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(_renderer, textSurface); // tạo kết cấu bề mặt
		if (textTexture == nullptr)
		{
			cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << endl;
		}
		else
		{
			SDL_Rect renderPos = { x - (textSurface->w / 2) , y  - (textSurface->h / 2), textSurface->w, textSurface->h }; //{ (640 - textSurface->w )/ 2, (480 - textSurface->h) / 2, textSurface->w, textSurface->h };
			SDL_RenderCopy(_renderer, textTexture, NULL, &renderPos);
		}
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);
	}
	TTF_CloseFont(font);
}

SDL_Texture* IO::loadAndScaleImage(const string& path, SDL_Renderer* _renderer)//, int newWidth, int newHeight)
{
    SDL_Surface* loadedSurface = IMG_Load(path.c_str()); // tải hình ảnh vào bề mặt
    if (loadedSurface == nullptr) {
        cout << "Unable to load image! IMG_Error: " << IMG_GetError() << endl;
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(_renderer, loadedSurface);
    if (texture == nullptr) {
        cout << "Unable to create texture from surface! SDL Error: " << SDL_GetError() << endl;
		return nullptr;
    }
    SDL_FreeSurface(loadedSurface);
    return texture;
}

void IO::Image(string image, const int x, const int y, const int width, const int height)
{
		SDL_Rect dstrect = { x, y, width, height };
		SDL_Texture* imageTexture = loadAndScaleImage(image, _renderer);//, 640 , 480 );
		SDL_RenderCopy(_renderer, imageTexture, NULL, &dstrect);
		SDL_DestroyTexture(imageTexture);
}

int IO::Menu()
{
	SDL_Surface* menuSurface = IMG_Load(Start);
    if (menuSurface == NULL)
    {
        cerr << "Failed to load image! SDL_Error: "<< SDL_GetError() << endl;
        return 0;
    }
    SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(_renderer, menuSurface);
    if (imageTexture == NULL)
    {
        cerr << "Failed to create texture! SDL_Error:"<< SDL_GetError() << endl;
        return 0;
    }
        SDL_FreeSurface(menuSurface);
        SDL_RenderCopy(_renderer, imageTexture, NULL, NULL); 
		
		SDL_Color textColor = { 255, 0, 0 };   
        this->WriteText("Press SPACE to Start Game",90, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 130, textColor);
		this->WriteText("Press Q to Quit Game",70, SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 130 + 50, textColor);
        
		SDL_RenderPresent(_renderer);
		SDL_RenderClear(_renderer);
		SDL_DestroyTexture(imageTexture);
      	
		bool quit = false;
		while(!quit)
		{
			int Keypressed = this->GetKeyPress();
			switch (Keypressed)
			{
				case SDLK_q:
		    		quit = true;
				break;
			
				case SDLK_SPACE:	
					return 1;	    
				break;
			}
		}
		SDL_DestroyRenderer(_renderer);
		SDL_DestroyWindow(_window);
		SDL_Quit();
		return 0;
}
	
bool IO::ReStart()
{
	bool quit = false;
		while(!quit)
		{
			int Keypressed = this->GetKeyPress();
			switch (Keypressed)
			{
				case SDLK_r:
		    		quit = true;
				break;
			
				case SDLK_q:	
					return false;	    
				break;
			}
		}
	return true;
}

void IO::Music(const string &path, int i, bool quit)
{
	//quit = false;
	Mix_Music *backgroundMusic = Mix_LoadMUS(path.c_str());
        if (backgroundMusic == NULL)
		{
       		cerr << "Failed to load beat music! SDL_mixer Error: %s" << Mix_GetError();
       		Mix_CloseAudio();
    	}
		Mix_PlayMusic(backgroundMusic, i);
	if(quit)
	{
		Mix_FreeMusic(backgroundMusic);
    	backgroundMusic = NULL;
	}
}