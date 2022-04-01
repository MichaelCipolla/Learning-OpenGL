#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

/// <summary>
/// Whenever the window changes in size, GLFW will call this function and fill in the arguments with the 
/// parameters passed into this function.
/// </summary>
/// <param name="window">Window that the event is listening to</param>
/// <param name="width">New width of the window</param>
/// <param name="height">New height of the window</param>
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

/// <summary>
/// Processes user input associated with the window.
/// </summary>
/// <param name="window">Window that user input is being processed on.</param>
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}

int main() {
	// First steps: initialize GLFW (library that handles all of the OS specific implementations of OpenGL) and configure it with the glfwWindowHint function.
	// 
	// Context Version Major and Minor reflect the version of GLFW we are using (3.3) and we specify that we are using the Core Profile which gives us a smaller subset
	// of OpenGL features without backwards-compatible features we no longer need. (pg. 20)

	// running the OpenGL Extension Viewer reveals that the current version of OpenGL that my graphics card is using is version 4.6.

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHInt(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // this line will add compatibility for MacOS

	// Create a new window object, which will hold all of the windowing data
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLAD and pass the function to load the address of OpenGL function pointers that are OS-specific. (pg. 21)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600); // tells OpenGL the size of the rendering window (first 2 parametrs are the location of the lower left corner of the window. 3rd and 4th parameter are the width and height in pixels) (pg. 21)
	
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // register the callback function we wrote to the window resizing event. (pg. 22)

	// Create the render loop: (pg 22.)
	// The glfwWindowShouldClose function checks at the start of each loop iteration if GLFW has been instructed to close. If so, function returns true and the render loop exits
	// glfwSwapBuffers will swap the color buffer that is used to render to during this render iteration and show it as output to the screen. (pg. 23)
	// The glfwPollEvents function checks if any events are triggered, updates the window state, and then calls any corresponding funcitons (which were registered via callback methods) (pg. 22)
	//
	// At the start of the frame you want to clear the screen, otherwise you would still see the results from the previous frame... (pg. 24)
	while (!glfwWindowShouldClose(window)) {
		// input
		processInput(window);

		// rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT); 

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate(); // this function properly cleans up / deletes all of GLFW's resources that were allocated.
	return 0;
}