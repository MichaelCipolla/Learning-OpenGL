#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

#include "Shaders.h"
#include "stb_image.h"
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

bool isWireFrame = false;

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame
float specularStrength = 0.5f;

float rComponent = 1.0f;
float gComponent = 1.0f;
float bComponent = 1.0f;

bool isToggleMovement = false;

float lastXPos = 0.0f;
float lastYPos = 0.0f;
bool firstMouse = true;

glm::vec3 lightPos(1.2f, 1.0f, -4.0f);

Camera* camera;


void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	camera->ProcessMouseScroll(yoffset);
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (firstMouse) {
		lastXPos = xpos;
		lastYPos = ypos;
		firstMouse = false;
	}

	float xOffset = lastXPos - xpos;
	float yOffset = lastYPos - ypos;

	lastXPos = xpos;
	lastYPos = ypos;

	camera->ProcessMouseMovement(xOffset, yOffset);
}



void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // this function returns GLFW_RELEASE if the key is not pressed
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
		isWireFrame = !isWireFrame;
		if (isWireFrame) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W)) {
		camera->ProcessKeyboard(Camera_Movement::FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S)) {
		camera->ProcessKeyboard(Camera_Movement::BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A)) {
		camera->ProcessKeyboard(Camera_Movement::LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D)) {
		camera->ProcessKeyboard(Camera_Movement::RIGHT, deltaTime);
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		rComponent += deltaTime * 1.0f;
		if (rComponent > 1.0f) {
			rComponent = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
		gComponent += deltaTime * 1.0f;
		if (gComponent > 1.0f) {
			gComponent = 1.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS) {
		bComponent += deltaTime * 1.0f;
		if (bComponent > 1.0f) {
			bComponent = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
		rComponent -= deltaTime * 1.0f;
		if (rComponent < 0.0f) {
			rComponent = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) {
		gComponent -= deltaTime * 1.0f;
		if (gComponent < 0.0f) {
			gComponent = 0.0f;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
		bComponent -= deltaTime * 1.0f;
		if (bComponent < 0.0f) {
			bComponent = 0.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		isToggleMovement = !isToggleMovement;
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

	Shader* objectShader = new Shader("Shaders/Vertex/vertexShader.v", "Shaders/Fragment/fragmentShader.f");

	Shader* lightSourceShader = new Shader("Shaders/Vertex/lightSourceShader.v", "Shaders/Fragment/lightSourceShader.f");
	/*unsigned int shaderProgram = shaderLoader->createShaderProgram("Shaders/Vertex/LightingVertexShader.v", "Shaders/Fragment/LightingFragmentShader.f");*/

	float vertices[] = {
		// positions			// normals				// texture coords
		-0.5f,	-0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	1.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	1.0f,	1.0f,
		-0.5f,	0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,	0.0f,	0.0f,	-1.0f,	0.0f,	0.0f,

		-0.5f,	-0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	1.0f,	1.0f,
		-0.5f,	0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,	0.0f,	0.0f,	1.0f,	0.0f,	0.0f,

		-0.5f,	0.5f,	0.5f,	-1.0f,	0.0f,	0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,	-1.0f,	0.0f,	0.0f,	1.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,	-1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	-0.5f,	-1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		-0.5f,	-0.5f,	0.5f,	-1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,	-1.0f,	0.0f,	0.0f,	1.0f,	0.0f,

		0.5f,	0.5f,	0.5f,	1.0f,	0.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,	-0.5f,	1.0f,	0.0f,	0.0f,	1.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,	1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,	1.0f,	0.0f,	0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,	1.0f,	0.0f,	0.0f,	0.0f,	0.0f,
		0.5f,	0.5f,	0.5f,	1.0f,	0.0f,	0.0f,	1.0f,	0.0f,

		-0.5f,	-0.5f,	-0.5f,	0.0f,	-1.0f,	0.0f,	0.0f,	1.0f,
		0.5f,	-0.5f,	-0.5f,	0.0f,	-1.0f,	0.0f,	1.0f,	1.0f,
		0.5f,	-0.5f,	0.5f,	0.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	-0.5f,	0.5f,	0.0f,	-1.0f,	0.0f,	1.0f,	0.0f,
		-0.5f,	-0.5f,	0.5f,	0.0f,	-1.0f,	0.0f,	0.0f,	0.0f,
		-0.5f,	-0.5f,	-0.5f,	0.0f,	-1.0f,	0.0f,	0.0f,	1.0f,

		-0.5f,	0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	1.0f,
		0.5f,	0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	1.0f,
		0.5f,	0.5f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,
		0.5f,	0.5f,	0.5f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,
		-0.5f,	0.5f,	0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	0.0f,
		-0.5f,	0.5f,	-0.5f,	0.0f,	1.0f,	0.0f,	0.0f,	1.0f
	};



	int numberOfCubes = 2;
	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f, 0.0f, 0.0f)
	};

	unsigned int VAO1; // This stores vertexAttribute calls 
	glGenVertexArrays(1, &VAO1);
	glBindVertexArray(VAO1);

	// This is a buffer on the GPU Hardware that stores vertex information. We need to send data to this buffer on the GPU
	// in order for the shader to have data to work with.
	unsigned int VBO1; // This stores vertex information
	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// VertexAttribPointer information:
	// These pointers are stored in vertex attribute arrays. This allows us to store different attribute pointers for different vertex buffer objects.
	// 
	// This pointer is used to keep track of specific data in the buffer. The parameters are as follows:
	//		1. This parameter acts as the id for the group of data. You can reference this data in the vertex shader using layout (location = {id}).
	//		2. This parameter specifies how many values are in this cluster of data on the buffer.
	//		3. This parameter specifies the data type of the data being stored in the buffer.
	//		4. This parameter specifies whether or not the data should be "normalized" -- not entirely sure what this does, but it's recommended to leave it to false
	//		5. This parameter specifies the span of the data, i.e. how many bytes away from the start position you have to traverse until you reach the start of the 
	//		   next grouping of data in the buffer.
	//		6. This parameter specifies the first occurance of your data (in bits) in the buffer. If you have the starting point and the span between your data, you know 
	//		   how far to traverse before you can start reading data of your specified type again.
	//

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // vertexAttribute pointers can only be initialized after a VBO is defined
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO1);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glEnable(GL_DEPTH_TEST);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	camera = new Camera();

	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);

	while (!glfwWindowShouldClose(window)) {
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view;

		view = camera->GetViewMatrix();

		glm::mat4 projection;
		projection = glm::perspective(glm::radians(camera->Zoom), 800.0f / 600.0f, 0.1f, 100.0f);

		objectShader->use();
		objectShader->setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		objectShader->setVec3("lightColor", rComponent, gComponent, bComponent);
		objectShader->setVec3("lightPos", lightPos);
		objectShader->setVec3("viewPos", camera->Position);
		objectShader->setFloat("specularStrength", specularStrength);

		int viewLoc = glGetUniformLocation(objectShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		int projLoc = glGetUniformLocation(objectShader->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(VAO1);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, cubePositions[0]);
		int modelLoc = glGetUniformLocation(objectShader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);

		// -------------------------------------------------------------
		lightSourceShader->use();
		lightSourceShader->setVec3("rbgVector", rComponent, gComponent, bComponent);
		model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));

		//Every pass, we should update lightPos:
		if (isToggleMovement) {
			float t = currentFrame;
			lightPos.x = 2 * cos(t);
			lightPos.z = 2 * sin(t);
		}

		viewLoc = glGetUniformLocation(lightSourceShader->ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		projLoc = glGetUniformLocation(lightSourceShader->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		modelLoc = glGetUniformLocation(lightSourceShader->ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate(); // this function properly cleans up / deletes all of GLFW's resources that were allocated.
	return 0;
}