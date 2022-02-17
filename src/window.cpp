#include <stdio.h>
#include "window.hpp"

static void glfw_error_callback(int error, const char* description){
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

Window::Window(bool fullscreen){
	// Setup window
	glfwSetErrorCallback(glfw_error_callback);
	if(!glfwInit()){
		throw "GLFW initialization failed";
	}

	// Decide GL+GLSL versions
#if defined(__APPLE__)
	// GL 3.2 + GLSL 150
	const char* glsl_version = "#version 150";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Required on Mac
#else
	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// 3.0+ only
#endif

	// Create window with graphics context
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	if(fullscreen){
		window = glfwCreateWindow(mode->width,mode->height,"xavine",monitor,NULL);
	}else{
		window = glfwCreateWindow(mode->width,mode->height,"xavine",NULL,NULL);
	}
	if(window == NULL){
		throw "could not create GLFW window";
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync
}

Window::~Window(){
	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::render(){
	glfwPollEvents();
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
}

bool Window::closed(){
	return glfwWindowShouldClose(window);
}
