#include <iostream>
#include <shader.h>
// GLAD
#include <glad/glad.h> 
#include <GLFW\glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <filesystem>
//hello, how are ytou?


const char* vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout(location = 1) in vec3 aColor;\n"
"layout(location = 2) in vec2 aTexCoord;\n"
"out vec3 ourColor;\n"
"out vec2 TexCoord;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos, 1.0);\n"
" ourColor = aColor;\n"
"    TexCoord = aTexCoord;\n"
"}\n";


const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"in vec2 TexCoord;\n"
"uniform sampler2D ourTexture;\n"
"void main()\n"
"{\n"
"FragColor = texture(ourTexture, TexCoord);\n"
"}\n";


/*
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos aP.x,os.y, aPos.z, 1.0);\n"
"   ourColor = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = ourColor;\n"
"}\n\0";
*/




const GLuint WIDTH = 800, HEIGHT = 600;


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}


void useUniform(GLuint shaderProgram) {

	double  timeValue = glfwGetTime();
	float myOffset = .7f;


	int vertexPosLocation = glGetUniformLocation(shaderProgram, "ourOffset");
	glUniform1f(vertexPosLocation, myOffset);

}


void loadTexture(GLuint& texture) {
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	int width, height, nrChannels;

	unsigned char* data = stbi_load("REDACTED)",
		&width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Failed to load: " << stbi_failure_reason() << "\n";
	}
	stbi_image_free(data);


}


void setupTriangle(const float* vertices, const int *indicies, 
	GLuint& VAO, GLuint& VBO, GLuint& EBO, size_t vertCount, size_t indicCount) {



	printf("THIS IS THE VERCOUNT: [%d]", vertCount);
	printf("THIS IS THE indicCount: [%d]", indicCount);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, (vertCount) * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indicCount, indicies, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}





void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

int main(int argc, char* argv[])
{



	/*
	float vertices[] = {
	   -0.9f, -0.5f, 0.0f,  // left
	   -0.0f, -0.5f, 0.0f,  // right
	   -0.45f, 0.5f, 0.0f,  // top
	};

	*/
	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	 int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetKeyCallback(window, key_callback);
	glfwMakeContextCurrent(window);

	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK)
	//{
	//	std::cout << "Failed to initialize GLEW" << std::endl;
	//	return -1;
	//}
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	printf("MADE IT BEFORE SHADER");
	Shader ourShader("4.1.texture.vs", "4.1.texture.fs");
	printf("MADE IT AFTER SHADER");

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	/// main code

	// Shader (compile->link->delete)
	GLuint vertexShader, fragmentShader, fragmentShader2, shaderProgram, shaderProgram2;
	// Vertex Shader
	vertexShader = glCreateShader(GL_VERTEX_SHADER);




	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);






	// is vertexShader compile success?
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}






	// is fragmentShader compile success?
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Shader Program
	shaderProgram = glCreateProgram();


	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);









	// is shaderProgram link success?
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//start here

	//buffer object


	Shader ourShader("3.3.shader.vs", "3.3.shader.fs"); // you can name your shader files however you like

	unsigned  int VAO = 0;
	unsigned int VBO = 0;
	unsigned int EBO = 0;

	unsigned int texture = 0;
	//vertex array ohject



	
	setupTriangle(vertices, indices, VAO, VBO, EBO,
		sizeof(vertices) / sizeof(float),
		sizeof(indices) / sizeof(int));

	loadTexture(texture);

	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		ourShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);


	/// main code end

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	std::cout << key << std::endl;
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
