#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "my_imgui.h"
class Window{
	public:
		Window(bool fullscreen);
		~Window();
		void render();
		bool closed();
	private:
		GLFWwindow* window;
};
