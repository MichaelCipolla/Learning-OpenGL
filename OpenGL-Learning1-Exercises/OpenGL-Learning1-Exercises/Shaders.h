#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

class ShaderLoader {
private:

	unsigned int currentShaderProgram;
	std::vector<unsigned int> shaderIds;

	//shaders
	const char* vertexShaderSource = "#version 330 core\n"
		"layout (location = 0) in vec3 aPos;\n"
		"void main() {\n"
		"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
		"}\0";

	const char* fragmentShaderSource = "#version 330 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";

public:

	ShaderLoader();
	void CompileShader(unsigned int& shaderId, const char* shaderSource, int shaderType);
	void AttachShader(unsigned int& shaderProgram, unsigned int* shaderArray, int shaderArraySize);
	unsigned int GetCurrentShaderProgram();
};