#pragma once
#include "SDL2/SDL.h"
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include <string>
#include <iostream>

using namespace std;

enum Color { RED, BLUE, GREEN, ORANGE, CYAN, YELLOW, MAGENTA, AMBER, GRAY, WHITE };

class IO
{
public:
	IO();
	~IO();

	void InitalizeGraphics();
	void ClearScreen();
	void UpdateScreen();
	void DrawRectangle(const int x, const int y, const int width, const int height, const Color color);
	void DrawRectangleOutline(const int x, const int y, const int width, const int height, const Color color);
	int GetKeyPress();
	void WriteText(string text, const int t,const int x, const int y, SDL_Color color);
	void Image(string image, const int x, const int y, const int width, const int height);
	int Menu();
	bool ReStart();
	void Music(const string &path, int i, bool quit);

private:
	SDL_Window* _window;
	SDL_Renderer* _renderer;
	SDL_Texture* loadAndScaleImage(const string& path, SDL_Renderer* _renderer);//, int newWidth, int newHeight);
    void SetRendererColor(const Color color);
 };