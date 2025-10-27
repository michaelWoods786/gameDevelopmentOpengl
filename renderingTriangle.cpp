#include <iostream>
#include <shader.h>
// GLAD
#include <glad/glad.h> 
#include <GLFW\glfw3.h>




const char* vertexShaderSource =
"#version 330 core\n"

"layout(location = 0) in vec3 aPos;\n" // the position variable has attribute position 0
"uniform float ourOffset;\n"
"out vec4 vertexColor;\n" // specify a color output to the fragment shader
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x + ourOffset, aPos.y, aPos.z,  1.0);\n" // see how we directly give a vec3 to vec4's constructor
"vertexColor = gl_Position;\n" // set the output variable to a dark-red color
"}\n";

const char* fragmentShaderSource =
"#version 330 core\n"
"out vec4 FragColor;\n"
"in vec4 vertexColor;\n"
"void main()\n"
"{\n"
"FragColor = vertexColor;\n"
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




void useUniform(GLuint shaderProgram) {

	double  timeValue = glfwGetTime();
	float myOffset = .7f;
	
	
	int vertexPosLocation = glGetUniformLocation(shaderProgram, "ourOffset");
	glUniform1f(vertexPosLocation,  myOffset);

}





void setupTriangle(const float* vertices,
	GLuint& VAO, GLuint& VBO, size_t vertCount) {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(float), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
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

	
	//vertex array ohject

	setupTriangle(vertices, VAO, VBO, sizeof(vertices) / sizeof(float));




	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		/// ---main loop---
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		ourShader.use();

		
		glUseProgram(shaderProgram);
		

		useUniform(shaderProgram);


		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);
	
		glfwSwapBuffers(window);
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

