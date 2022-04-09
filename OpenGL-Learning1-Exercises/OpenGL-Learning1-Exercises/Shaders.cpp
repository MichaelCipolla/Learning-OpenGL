#include "Shaders.h"

unsigned int ShaderLoader::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	unsigned int vertexShader;
	compileShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);

	unsigned int fragmentShader;
	compileShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);

	unsigned int shaderIDs[2] = { vertexShader, fragmentShader };

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	int shaderArraySize = 2;

	attachShader(shaderProgram, shaderIDs, shaderArraySize);
	activeShaderPrograms.push_back(shaderProgram);
	return shaderProgram;
}

void ShaderLoader::compileShader(unsigned int &shaderId, const char* shaderSource, int shaderType) {
	shaderId = glCreateShader(shaderType);
	shaderIds.push_back(shaderId);
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl << std::endl;
	}
}

void ShaderLoader::attachShader(unsigned int& shaderProgram, unsigned int *shaderArray, int shaderArraySize) {
	for (int i = 0; i < shaderArraySize; i++) {
		glAttachShader(shaderProgram, shaderArray[i]);
	}
	glLinkProgram(shaderProgram);
	

	int success;
	char infoLog[512];

	for (int i = 0; i < shaderArraySize; i++) {
		glDeleteShader(shaderArray[i]);
	}

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}
}

bool ShaderLoader::deleteActiveShaderProgram(unsigned int activeShader) {
	for (int i = 0; i < activeShaderPrograms.size(); i++) {
		if (activeShaderPrograms[i] == activeShader) {
			glDeleteProgram(activeShaderPrograms[i]);
			activeShaderPrograms.erase(activeShaderPrograms.begin() + i);
			return true;
		}
	}
	return false;
}

bool ShaderLoader::clearActiveShaderPrograms() {
	for (int i = 0; i < activeShaderPrograms.size(); i++) {
		glDeleteProgram(activeShaderPrograms[i]);
	}
	activeShaderPrograms.clear();
	return activeShaderPrograms.empty();
}