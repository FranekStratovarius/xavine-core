#ifndef MY_IMGUI
#define MY_IMGUI

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

/*

initialize dear imgui

*/
void setup_my_imgui(const char* glsl_version, GLFWwindow* window);

#endif
