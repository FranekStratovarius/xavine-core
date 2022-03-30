#ifndef SHADER_HPP
#define SHADER_HPP

class Shader{
	public:
		unsigned int ID;
		Shader(const char* vertexPath, const char* fragmentPath);
		void use();
		void setBool(const char* &name, bool value);
		void setInt(const char* &name, int value);
		void setFloat(const char* &name, float value);
	private:
		void checkCompileErrors(unsigned int shader, const char* type);
};

#endif
