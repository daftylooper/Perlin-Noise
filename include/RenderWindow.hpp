#pragma once
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<vector>

class RenderWindow{
public:
	RenderWindow(const char* p_title, int p_w, int p_h);
	SDL_Texture* loadTexture(const char* p_filePath);
	void cleanUp();
	void clear();
	void render(std::vector<std::vector<double>> vals);
	void display();
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};