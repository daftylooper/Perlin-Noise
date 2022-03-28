#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h)
	:window(NULL), renderer(NULL)
{
	window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, SDL_WINDOW_SHOWN);

	if(window == NULL){
		std::cout<<"Hey.. Window failed to init. Error: "<<SDL_GetError()<<std::endl;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath){
	SDL_Texture* texture = NULL;
	texture = IMG_LoadTexture(renderer, p_filePath);

	if(texture == NULL){
		std::cout<<"Hey.. Failed to load texture. Error: "<<SDL_GetError()<<std::endl;
	}

	return texture;
}

void RenderWindow::cleanUp(){
	SDL_DestroyWindow(window);
}

void RenderWindow::clear(){
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

int scale(float val){
	return static_cast<int>(val*(255/2));
}

void RenderWindow::render(std::vector<std::vector<double>> vals){

	int valsizex = vals[0].size();
	int valsizey = vals.size();

	for(int i=0; i<valsizex; i++){
		for(int j=0; j<valsizey; j++){
			SDL_SetRenderDrawColor(renderer, scale(vals[i][j]), scale(vals[i][j]), scale(vals[i][j]), 255);
			SDL_RenderDrawPoint(renderer, i, j);
		}
	}
}

void RenderWindow::display(){
	SDL_RenderPresent(renderer);
}