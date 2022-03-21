#include <stdio.h>
#include "graphics/window.hpp"

int main(int, char**){

	//create window
	Window* window;
	try{
		window = new Window(false);
	}catch(const char* message){
		fprintf(stderr,message);
		return 1;
	}

	if(window==nullptr){
		fprintf(stderr,"no window");
	}

	//main loop
	while(!window->closed()){
		window->poll_events();
		window->render();
	}

	delete window;

	return 0;
}
