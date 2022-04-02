#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

/// <summary>
/// (pg. 30)
/// This is vertex shader source code that was written in the shader language OpenGLShadingLanguage (GLSL). We create a vector3 object to store position information
/// for the vertex and then cast it into the constructor of a vector4 to add a dimension called perspective division, which we set to 1.0. This code was then stored
/// as const char* so that we can reference it and pass it into the compiler function to compile the shader at runtime.
/// </summary>
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
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) { // this function returns GLFW_RELEASE if the key is not pressed
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

	/// <summary>
	/// (pg. 28)
	/// Vertices that are passed to the graphics card and process by openGL need to be defined in Normal Coordinate Form (NCF). This means that the range of the coordinates that
	/// are rendered only spans from [-1f, 1f]. Anything exceeding that range will be "clipped" and not rendered. All components of the vertex (x, y, z) must be in NCF or else
	/// the point will not render.
	/// </summary>
	/// <returns></returns>
	/*float vertices[]{
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f, 0.5f, 0.0f,
	};*/

	float vertices[] = {
		// first triangle
		0.5f, 0.5f, 0.0f, // top right
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, 0.5f, 0.0f, // top left
		// second triangle
		0.5f, -0.5f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f, // bottom left
		-0.5f, 0.5f, 0.0f // top left
	};


	/// <summary>
	/// (pg. 29)
	/// Here we define the Vertex Buffer Object. We bind this ID to the GL_ARRAY_BUFFER type, and we use the glBufferData type to send the data we defined in vertices into the specified
	/// buffer (GL_ARRAY_BUFFER). We then set the final parameter GL_STATIC_DRAW.
	/// 
	/// There are 3 forms this final parameter can take:
	/// 
	/// GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
	/// GL_STATIC_DRAW: the data is set only once and used many times.
	/// GL_DYNAMIC_DRAW: the data is changed alot and used many times.
	/// </summary>
	/// <returns></returns>
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	/// <summary>
	/// (pg. 31)
	/// Here, we create a vertex shader object, and attach it to the shader source code we wrote. We define the number of strings used to define our shader source code (we used 1 to store the program)
	/// and we compile the source code at run time.
	/// 
	/// In order to check whether or not the compilation was successful we create an int to store whether or not the compilation status was a success or not and print an error message
	/// if it was not successful.
	/// </summary>
	/// <returns></returns>
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	/// <summary>
	/// We define a shader program which will link the 2 shaders we wrote into a program that we can use to execute the shader rendering pipeline.
	/// The program, once linked to the 2 shaders will pass the output from one shader into the input of another shader. It is important that those
	/// inputs and outputs have matching formatting or you will get a LNK error.
	/// 
	/// Once the program has been linked, we no longer need the shader objects we created, so we delete them.
	/// </summary>
	/// <returns></returns>
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(shaderProgram); // Every shader and rendering call after glUseProgram will now use this program object (and thus the shaders).

	/// <summary>
	/// (pg. 34)
	/// Our vertex buffer data was formatted as a tightly packed array of vertices that had a size of 3. glVertexAttribPointer has quite a few parameters that help us 
	/// inform openGL of how we formatted that array.
	/// 
	/// Parameter1: The first parameter specifies which vertex attribute we want to configure. Remember that we specifed the location of the position vertex attriubute in the vertex
	///		shader with the following code: layout (location = 0). This sets the location of the vertex attribute to 0 an since we want to pass data to this vertex attribute, we pass
	///		in 0.
	/// Parameter2: The next argument specifies the size of the vertex attribute. The vertex attribute is a vec3 so it is composed of 3 values.
	/// Parameter3: The third argument specifies the type of data which is GL_FLOAT (a vec* in GLSL consists of floating point values).
	/// Parameter4: The next argument specifies if we want the data to be normalized. If we're inputting integer data types (int, byte) and we've set this to GL_TRUE, the integer data is
	///		normalized to 0 (or -1 for signed data) and 1 when converted to float. This is not relevant for us so we'll leave this at GL_FALSE.
	/// Parameter5: The fifth argument is known as the stride and tells us the space between consecutive vertex attributes. Since the next set of position data is located exactly
	///		3 times the size of a float away we specify that values as the stride. Note that since we know that the array is tightly packed (there is no space between the next vertex attribute
	///		value) we could've also specified the stride as 0 to let OpenGL determine the stride (this only works when values are tightly packed). Whenever we have more vertex attributes
	///		we have to carefully define the spacing between each vertex attribute but we'll get to see more examples of that later on.
	/// Parameter6: The last parameter is of type void* and thus requires that weird cast. This is the offset of where the position data begins in the buffer. Since the position data is
	///		at the start of the data array this value is just 0. We will explore this parameter in more detail later on.
	/// </summary>
	/// <returns></returns>
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//	Drawing out first object in OpenGL:
	//	0. copy our vertices array in a buffer for OpenGL to use
	//	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	//	1. then set the vertex attributes pointers
	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	//	glEnableVertexAttribArray(0);
	//	2. use our shader program when we want to render an object
	//	glUseProgram(shaderProgram);
	//	3. now draw the object
	//	someOpenGLFFunctionThatDrawsOutTriangle();

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate(); // this function properly cleans up / deletes all of GLFW's resources that were allocated.
	return 0;
}