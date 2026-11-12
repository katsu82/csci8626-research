
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "particle.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
unsigned int shaderProgram;
unsigned int VBO, VAO, EBO;//Array objectsVAO;

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform mat4 M;\n"
"uniform mat4 V;\n"
"uniform mat4 P;\n"
"void main()\n"
"{\n"
"	gl_Position = P * V * M * vec4(aPos, 1.0);\n"
"}\0";

const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 color;\n"
"void main()\n"
"{\n"
"	FragColor = color;\n"
"}\n\0";

GLFWwindow* window;
glm::mat4 projection;
glm::mat4 view;
glm::mat4 model;
std::vector<Particle> particles;

void initializeParticles()
{
	for(int i = 0; i < 30; i++)
	{
		float randomPos1 = (10 * (float) rand() / (RAND_MAX + 1)) + -5;
		float randomPos2 = (10 * (float) rand() / (RAND_MAX + 1)) + -5;
		float randomPos3 = (10 * (float) rand() / (RAND_MAX + 1)) + -5;
		Vector3 pos = Vector3{randomPos1, randomPos2, randomPos3};

		float randomScale1 = (3 * (float) rand() / (RAND_MAX + 1)) + 0;
		float randomScale2 = (3 * (float) rand() / (RAND_MAX + 1)) + 0;
		float randomScale3 = (3 * (float) rand() / (RAND_MAX + 1)) + 0;
		Vector3 sc = Vector3{randomScale1, randomScale2, randomScale3};

		float randomOrient1 = (6.28 * (float) rand() / (RAND_MAX + 1)) + -3.14;
		float randomOrient2 = (6.28 * (float) rand() / (RAND_MAX + 1)) + -3.14;
		float randomOrient3 = (6.28 * (float) rand() / (RAND_MAX + 1)) + -3.14;
		Vector3 orient = Vector3{randomOrient1, randomOrient2, randomOrient3};

		float randomColorR = (1 * (float) rand() / (RAND_MAX + 1)) + 0;
		float randomColorG = (1 * (float) rand() / (RAND_MAX + 1)) + 0;
		float randomColorB = (1 * (float) rand() / (RAND_MAX + 1)) + 0;
		float randomColorA = (1 * (float) rand() / (RAND_MAX + 1)) + 0;
		glm::vec4 col = glm::vec4(randomColorR, randomColorG, randomColorB, randomColorA);
		particles.emplace_back(pos, sc, orient, col);
	}
}

bool initialize()
{

	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Avalanche Model", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		glfwTerminate();
		return false;
	}

	view = glm::lookAt(glm::vec3(0, 0, -5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	projection = glm::perspective(glm::radians(80.0f), float(SCR_WIDTH/SCR_HEIGHT), 1.0f, 400.0f);
	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 1, 0));

	//build and compile shader program
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	//check for shader compilation errors
	int success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	//check for shader compile errors
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}

	//link the shaders
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	//check for linking errors
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		1.0f, 1.0f, -0.0f,
		1.0f, -1.0f, -0.0f,
		-1.0f, -1.0f, -0.0f,
		-1.0f, 1.0f, -0.0f
	};

	unsigned int indices[] = {
		0, 1, 3, //first Triangle
		1, 2, 3 //second Triangle
	};

	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)nullptr);
	glEnableVertexAttribArray(0);

	return true;
}

void cleanup()
{
	glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
	
	glfwTerminate();
}

void render()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "P"), 1, GL_FALSE, &projection[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "V"), 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "M"), 1, GL_FALSE, &model[0][0]);

	for(auto& particle : particles)
	{
		glm::vec4 color = particle.getColor();
		glUniform4f(glGetUniformLocation(shaderProgram, "color"), color[0], color[1], color[2], color[3]);
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &particle.getModel()[0][0]);
	}

	//glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

	//check and call events and swap the color buffers
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void update()
{

}

int main()
{
	if(!initialize())
	{
		return -1;
	}

	glClearColor(0.25f, 0.5f, 0.75f, 1.0f);

	//render loop
	while(!glfwWindowShouldClose(window))
	{

		processInput(window);
		update();
		render();		
		
	}

	cleanup();

	return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}