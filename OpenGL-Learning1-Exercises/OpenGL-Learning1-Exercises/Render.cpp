#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "Shaders.h"

////shaders (RGBA)
//const char* vertexShader = "#version 330 core\n"
//	"layout (location = 0) in vec3 aPos;\n"
//	"void main() {\n"
//	"	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//	"}\0";
//
//const char* fragmentShaderOrange = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"void main() {\n"
//	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
//	"}\0";
//
//const char* fragmentShaderYellow = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"void main() {\n"
//	"	FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
//	"}\0";
//
//const char* advancedVertexShader = "#version 330 core\n"
//	"layout(location = 0) in vec3 aPos; // position has attribute position 0\n"
//	"layout(location = 1) in vec3 aColor; // color has attribute position 1\n"
//	"out vec3 ourColor; // specify a color output to the fragment shader\n"
//	"void main() {\n"
//	"	gl_Position = vec4(aPos, 1.0); // we give a vec3 to vec4’s constructor\n"
//	"	ourColor = aColor; // output variable to dark-red\n"
//	"}\0";
//
//const char* advancedFragmentShader = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"in vec3 ourColor; // input variable from vs (same name and type)\n"
//	"void main() {\n"
//	"	FragColor = vec4(ourColor, 1.0);\n"
//	"}\0";
//
//const char* uniformFragmentShader = "#version 330 core\n"
//	"out vec4 FragColor;\n"
//	"uniform vec4 ourColor;\n"
//	"void main() {\n"
//	"	FragColor = vec4(ourColor, 1.0);\n"
//	"}\0";

bool isWireFrame = false;

void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // this function returns GLFW_RELEASE if the key is not pressed
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		isWireFrame = !isWireFrame;
		if (isWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
}

int main() {
	//init steps
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Exercise", NULL, NULL);

	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	// openGL functions can now be used beyond this point:
	glViewport(0, 0, 800, 600);

	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	
	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;


	ShaderLoader* shaderLoader = new ShaderLoader();
	unsigned int shaderProgram = shaderLoader->createShaderProgram("Shaders/Vertex/learningVertexShader.v", "Shaders/Fragment/learningFragmentShader.f");

	float vertices[] = {
		 0.0f, 0.5f, 0.0f, // pos...
		 1.0f, 0.0f, 0.0f, // color...

		 -0.5f, -0.5f, 0.0f,
		 0.0f, 1.0f, 0.0f,

		 0.5f, -0.5f, 0.0f,
		 0.0f, 0.0f, 1.0f,
	};

	int vertexOrdering[] = {
		0, 1, 2,
		2, 3, 4
	};

	unsigned int VAO1; // This stores vertexAttribute calls 
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);
	

	unsigned int VBO1; // This stores vertex information
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // vertexAttribute pointers can only be initialized after a VBO is defined
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	int vertexOrdering1[] = {
		0, 1, 2,
	};

	unsigned int EBO1;
	glGenBuffers(1, &EBO1);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertexOrdering1), vertexOrdering1, GL_STATIC_DRAW);


	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		
		float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		shaderLoader->use();
		shaderLoader->setFloat("offset", greenValue);

		//EBO method:
		//glUseProgram(shaderProgram);
		glBindVertexArray(VAO1);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate(); // this function properly cleans up / deletes all of GLFW's resources that were allocated.
	return 0;
}