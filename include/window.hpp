#include <glad/gl.h>
#include <GLFW/glfw3.h> // Will drag system OpenGL headers
class Window{
	public:
		Window(bool fullscreen);
		~Window();
		void render();
		bool closed();
		void poll_events();
	private:
		GLFWwindow* window;
		GLuint vertex_buffer, vertex_shader, fragment_shader, program;
		GLint mvp_location, vpos_location, vcol_location;
		bool* show_demo_window = new bool(false);
		const int frames = 120;
		float* frametimes;
		double lasttime;
};
