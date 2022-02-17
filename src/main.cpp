#include <stdio.h>
#include "window.hpp"

int main(int, char**){

	//create window
	Window* window;
	try{
		window = new Window(false);
	}catch(const char* message){
		fprintf(stderr,message);
		delete window;
		return 1;
	}

	//main loop
	while (!window->closed()){
		window->render();
	}

	delete window;

	return 0;
}
