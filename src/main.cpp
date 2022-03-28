#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<iostream>
#include<vector>
#include<time.h>

#include "RenderWindow.hpp"

std::vector<std::vector<double>> perlin(int x, int y, int nx, int ny);

double dotProduct(std::vector<int> vec1, std::vector<double> vec2);

std::vector<double> distanceVector(std::vector<int> vec1, std::vector<double> vec2);

std::vector<std::vector<double>> interpolation(std::vector<std::vector<std::vector<int>>> gradientVec, std::vector<std::vector<std::vector<double>>> values);


int main(int argv, char* args[]){

	if(SDL_Init(SDL_INIT_VIDEO) > 0){
		std::cout<<"Hey.. SDL_Init HAS FAILED. SDL ERROR: "<<SDL_GetError()<<std::endl;
	}

	if(!IMG_Init(IMG_INIT_PNG)){
		std::cout<<"Hey.. IMG_Init HAS FAILED. SDL ERROR: "<<SDL_GetError()<<std::endl;
	}

	srand(time(0));

	std::vector<std::vector<double>> vals;
	vals = perlin(512, 512, 39, 39);

	RenderWindow window("PERLIN NOISE GENERATOR v1.0", 1280, 720);

	bool gameRunning = true;
	SDL_Event event;

	while(gameRunning){
		while(SDL_PollEvent(&event)){
			if(event.type==SDL_QUIT){
				gameRunning=false;
			}
		}

		window.clear();
		window.render(vals);
		window.display();
		SDL_Delay(50);
	}
	
	window.cleanUp();
	SDL_Quit();

	return 0;
}

std::vector<std::vector<double>> perlin(int x, int y, int nx, int ny){

	std::vector<std::vector<std::vector<double>>> values;
	std::vector<std::vector<std::vector<int>>> gradientVec;
	double alphax = 1/static_cast<double>(x);
	double alphay = 1/static_cast<double>(y);
	std::cout<<"\n"<<alphax<<" "<<alphay<<"\n";

	std::vector<std::vector<int>> gtemp;
	for(int i=0; i<(nx+2); i++){
		for(int j=0; j<(ny+2); j++){
			gtemp.push_back({((rand()%2==0)?1:-1), ((rand()%2==0)?1:-1)});
		}
		gradientVec.push_back(gtemp);
		gtemp.clear();
	}

	std::vector<std::vector<double>> temp;
	for(int i=0; i<x; i++){
		for(int j=0; j<y; j++){
			temp.push_back({alphax*i, alphay*j});
		}
		values.push_back(temp);
		temp.clear();
	}


	std::vector<std::vector<double>> vals;
	vals = interpolation(gradientVec, values);
	return vals;
}

double dotProduct(std::vector<int> vec1, std::vector<double> vec2){
	return static_cast<double>(vec1[0])*vec1[1] + static_cast<double>(vec1[1])*vec2[1];
}

std::vector<double> distanceVector(std::vector<int> vec1, std::vector<double> vec2){
	std::vector<double> distVec = {static_cast<double>(vec2[0])-vec1[0], static_cast<double>(vec2[1])-vec1[1]};
	return distVec;
}

std::vector<std::vector<double>> interpolation(std::vector<std::vector<std::vector<int>>> gradientVec, std::vector<std::vector<std::vector<double>>> values){


	std::vector<std::vector<double>> val;
	std::vector<double> temp;
	int valsizex = values[0].size();
	int valsizey = values.size();
	int gradientVecSizex = gradientVec[0].size();
	int gradientVecSizey = gradientVec.size();

	for(int i=0; i<valsizex; i++){
		for(int j=0; j<valsizey; j++){

			int x1 = i*(gradientVecSizex-1)/valsizex;
			int y1 = j*(gradientVecSizey-1)/valsizey;
			int x2 = x1+1;
			int y2 = y1+1;

			double dota = dotProduct(gradientVec[x1][y1], distanceVector(gradientVec[x1][y1], values[i][j]));
			double dotb = dotProduct(gradientVec[x2][y1], distanceVector(gradientVec[x2][y1], values[i][j]));
			double dotc = dotProduct(gradientVec[x1][y2], distanceVector(gradientVec[x1][y2], values[i][j]));
			double dotd = dotProduct(gradientVec[x2][y2], distanceVector(gradientVec[x2][y2], values[i][j]));

			double ab = dota + (values[i][j][0] - static_cast<int>(values[i][j][0]))*(dotb-dota);
			double cd = dotc + (values[i][j][0] - static_cast<int>(values[i][j][0]))*(dotd-dotc);

			temp.push_back(ab + (values[i][j][1]- static_cast<int>(values[i][j][1]))*(cd-ab));
		}

		val.push_back(temp);
		temp.clear();
	}
	return val;
}
