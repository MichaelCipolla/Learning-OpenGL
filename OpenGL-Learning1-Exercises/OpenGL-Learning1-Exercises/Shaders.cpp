#include "Shaders.h"

ShaderLoader::ShaderLoader() {
	unsigned int vertexShader;
	CompileShader(vertexShader, vertexShaderSource, GL_VERTEX_SHADER);


	unsigned int fragmentShader;
	CompileShader(fragmentShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
	
		
	unsigned int shaderIDs[2] = { vertexShader, fragmentShader };

	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();
	int shaderArraySize = 2;

	AttachShader(shaderProgram, shaderIDs, shaderArraySize);
	currentShaderProgram = shaderProgram;
}

void ShaderLoader::CompileShader(unsigned int &shaderId, const char* shaderSource, int shaderType) {
	shaderId = glCreateShader(shaderType);
	shaderIds.push_back(shaderId);
	glShaderSource(shaderId, 1, &shaderSource, NULL);
	glCompileShader(shaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
}

void ShaderLoader::AttachShader(unsigned int& shaderProgram, unsigned int *shaderArray, int shaderArraySize) {
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

unsigned int ShaderLoader::GetCurrentShaderProgram() {
	return currentShaderProgram;
}
