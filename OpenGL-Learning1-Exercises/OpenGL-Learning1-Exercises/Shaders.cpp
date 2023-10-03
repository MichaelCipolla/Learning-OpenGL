#include "Shaders.h"

unsigned int ShaderLoader::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	//ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		// open files
		vShaderFile.open(vertexShaderSource);
		fShaderFile.open(fragmentShaderSource);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e) {
 		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertexShaderID;
	unsigned int fragmentShaderID;

	compileShader(vertexShaderID, vShaderCode, GL_VERTEX_SHADER);
	compileShader(fragmentShaderID, fShaderCode, GL_FRAGMENT_SHADER);

	unsigned int shaderArray[2] = { vertexShaderID, fragmentShaderID };
	currentProgram = glCreateProgram();
	
	attachShader(currentProgram, shaderArray, 2);
	return currentProgram;
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
		std::cout << "ERROR::SHADER::" << (shaderType == 0x8B30 ? "FRAGMENT" : (shaderType == 0x8B31 ? "VERTEX" : "UNKOWN_SHADER")) << "COMPILATION_FAILED\n" << infoLog << std::endl << std::endl;
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

void ShaderLoader::use() {
	glUseProgram(currentProgram);
}

void ShaderLoader::setBool(const std::string& name, bool value) const {
	glUniform1i(glGetUniformLocation(currentProgram, name.c_str()), (int)value);
}
void ShaderLoader::setInt(const std::string & name, int value) const {
	glUniform1i(glGetUniformLocation(currentProgram, name.c_str()), value);
}
void ShaderLoader::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(currentProgram, name.c_str()), value);
}
void ShaderLoader::setVec3(const std::string& name, float value1, float value2, float value3) {
	glUniform3f(glGetUniformLocation(currentProgram, name.c_str()), value1, value2, value3);
}