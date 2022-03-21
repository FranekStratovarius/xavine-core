#include <stdio.h>
#include <stdlib.h>
#include "graphics/window.hpp"
#include "linmath.h"
#include "graphics/my_imgui.hpp"

static void glfw_error_callback(int error, const char* description){
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	Window* my_window = (Window*)glfwGetWindowUserPointer(window);
	if(my_window){
		my_window->my_key_callback(glfwGetKeyName(key,scancode),action);
	}
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

	glfwSetWindowUserPointer(window,this);

	glfwSetKeyCallback(window,key_callback);

	glfwMakeContextCurrent(window);
	gladLoadGL(glfwGetProcAddress);
	glfwSwapInterval(1); // Enable vsync

	shader = new Shader("vertex.glsl", "fragment.glsl");

	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	setup_my_imgui(glsl_version,window);

	frametimes = new float[frames];
	lasttime = glfwGetTime();

	luastate = load_lua();
	if(!luastate){
		throw "luastate not initialized";
	}
}

Window::~Window(){
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

	delete show_demo_window;
	delete[] frametimes;

	close_lua(luastate);

	cleanup_my_imgui();

	glfwDestroyWindow(window);
	glfwTerminate();
}

void Window::render(){
	for(int i=0;i<frames-1;i++){
			frametimes[i]=frametimes[i+1];
	}
	double newtime = glfwGetTime();
	frametimes[frames-1] = (float)newtime-lasttime;
	lasttime = newtime;

	float ratio;
	int width, height;
	mat4x4 m, p, mvp;

	glfwGetFramebufferSize(window, &width, &height);
	ratio = width / (float) height;

	//clear screen
	glViewport(0, 0, width, height);
	glClear(GL_COLOR_BUFFER_BIT);

	shader->use();
	//draw on screen
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	render_my_imgui(frametimes, frames, show_demo_window);

	//show screen
	glfwSwapBuffers(window);
}

bool Window::closed(){
	return glfwWindowShouldClose(window);
}

void Window::poll_events(){
	glfwPollEvents();

	run_lua(luastate);
}

void Window::my_key_callback(const char* key,int action){
	lua_key_pressed(luastate,key,action);
}
