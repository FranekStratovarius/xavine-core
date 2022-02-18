#include <stdio.h>
#include <stdlib.h>
#include "window.hpp"
#include "linmath.h"
#include "my_imgui.hpp"

static const struct
{
	float x, y;
	float r, g, b;
} vertices[3] =
{
	{ -0.6f, -0.4f, 1.f, 0.f, 0.f },
	{  0.6f, -0.4f, 0.f, 1.f, 0.f },
	{   0.f,  0.6f, 0.f, 0.f, 1.f }
};

static const char* vertex_shader_text =
"#version 110\n"
"uniform mat4 MVP;\n"
"attribute vec3 vCol;\n"
"attribute vec2 vPos;\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"	gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"	color = vCol;\n"
"}\n";

static const char* fragment_shader_text =
"#version 110\n"
"varying vec3 color;\n"
"void main()\n"
"{\n"
"	gl_FragColor = vec4(color, 1.0);\n"
"}\n";

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

	glGenBuffers(1, &vertex_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
	glCompileShader(vertex_shader);

	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
	glCompileShader(fragment_shader);

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	mvp_location = glGetUniformLocation(program, "MVP");
	vpos_location = glGetAttribLocation(program, "vPos");
	vcol_location = glGetAttribLocation(program, "vCol");

	glEnableVertexAttribArray(vpos_location);
	glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) 0);
	glEnableVertexAttribArray(vcol_location);
	glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]), (void*) (sizeof(float) * 2));
	
	setup_my_imgui(glsl_version,window);

	frametimes = new float[frames];
	lasttime = glfwGetTime();

	luastate = load_lua();
	if(!luastate){
		throw "luastate not initialized";
	}
}

Window::~Window(){
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

	mat4x4_identity(m);
	mat4x4_rotate_Z(m, m, (float) glfwGetTime());
	mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	mat4x4_mul(mvp, p, m);

	glUseProgram(program);
	glUniformMatrix4fv(mvp_location, 1, GL_FALSE, (const GLfloat*) mvp);
	//draw on screen
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
