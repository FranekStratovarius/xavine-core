#include <glad/gl.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
#include "lua.hpp"

class Window{
	public:
		Window(bool fullscreen);
		~Window();
		void render();
		bool closed();
		void poll_events();
		void my_key_callback(const char* key,int action);
	private:
		GLFWwindow* window;
		lua_State* luastate;

		GLuint vertex_buffer, vertex_shader, fragment_shader, program;
		GLint mvp_location, vpos_location, vcol_location;

		bool* show_demo_window = new bool(false);
		const int frames = 120;
		float* frametimes;
		double lasttime;
};
