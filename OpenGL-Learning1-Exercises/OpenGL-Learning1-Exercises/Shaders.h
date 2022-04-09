#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class ShaderLoader {
private:
	std::vector<unsigned int> activeShaderPrograms;
	std::vector<unsigned int> shaderIds;
	unsigned int currentProgram;

	void compileShader(unsigned int& shaderId, const char* shaderSource, int shaderType);
	void attachShader(unsigned int& shaderProgram, unsigned int* shaderArray, int shaderArraySize);

public:
	unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
	std::vector<unsigned int> getActiveShaderPrograms;
	bool deleteActiveShaderProgram(unsigned int activeShader);
	bool clearActiveShaderPrograms();

	void use();
	void setBool(const std::string& name, bool value) const;
	void setInt(const std::string& name, int value) const;
	void setFloat(const std::string& name, float value) const;
};