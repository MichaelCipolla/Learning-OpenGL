#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>

class ShaderLoader {
private:
	std::vector<unsigned int> activeShaderPrograms;
	std::vector<unsigned int> shaderIds;

	void compileShader(unsigned int& shaderId, const char* shaderSource, int shaderType);
	void attachShader(unsigned int& shaderProgram, unsigned int* shaderArray, int shaderArraySize);

public:
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	std::vector<unsigned int> getActiveShaderPrograms;
	bool deleteActiveShaderProgram(unsigned int activeShader);
	bool clearActiveShaderPrograms();
};