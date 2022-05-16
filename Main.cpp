#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Texture.h"
#include "shaderClass.h"
#include "VBO.h"
#include "EBO.h"
#include "VAO.h"


const unsigned int width = 800;
const unsigned int height = 800;


//Vertex coordinates
GLfloat vertices[] =
{//		COORDINATES				/		COLORS			/			TexCoord	//		
	-0.5f,	0.0f,	0.5f,		0.83f,	0.70f,	0.44f,		0.0f,	0.0f,
	-0.5f,	0.0f,	-0.5f,		0.83f,	0.70f,	0.44f,		5.0f,	0.0f,
	0.5f,	0.0f,	-0.5f,		0.83f,	0.70f,	0.44f,		0.0f,	0.0f,
	0.5f,	0.0f,	0.5f,		0.83f,	0.70f,	0.44f,		5.0f,	0.0f,
	0.0f,	0.8f,	0.0f,		0.92f,	0.86f,	0.76f,		2.5f,	5.0f,
};

GLuint indices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 1, 4,
	1, 2, 4,
	2, 3, 4,
	3, 0, 4
};

int main()
{
	//Initialize GLFW
	glfwInit();



	//Tell GLFW what version of OpenGL we're using
	//3.3 in this case
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW we are using the CORE profile
	//Which means we only have the modern functions
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);



	//Create window of size 800x800 named OpenGL Project
	GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Project", NULL, NULL);
	//Check if window failed to open
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	//Add window to current context
	glfwMakeContextCurrent(window);

	//Load GLAD so it configuers OpenGL
	gladLoadGL();

	//Specify the viewport of OpenGL in the window
	//The viewport here goes from (x,y) = (0,0) to (x,y) = (800, 800)
	glViewport(0, 0, width, height);

	//Create shader program
	Shader shaderProgram("default.vert", "default.frag");

	//Generate vertex array object and bind it
	VAO VAO1;
	VAO1.Bind();

	//Generate vertex buffer object and element buffer objects
	VBO VBO1(vertices, sizeof(vertices));
	EBO EBO1(indices, sizeof(indices));

	//Link VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	//Ubind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//Gets ID of uniform called scale
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture

	Texture popCat("brick.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	popCat.texUnit(shaderProgram, "tex0", 0);

	float rotation = 0.0f;
	double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	//While loop to prevent GLFW window from closing
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//Tell OpenGL which Shader Program to use
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60)
		{
			rotation += 0.5f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
		proj = glm::perspective(glm::radians(45.0f), (float)(width / height), 0.1f, 100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, 0.5f);
		popCat.Bind();
		//Bind the VAO so OpenGL knows to use it
		VAO1.Bind();
		//Draw the triangle using GL_TRIANGLES primative
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		//glDrawArrays(GL_TRIANGLES, 0, 3);
		glfwSwapBuffers(window);

		//Handle GLFW events
		glfwPollEvents();
	}

	//Delete all the objects we created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	popCat.Delete();
	shaderProgram.Delete();

	//Destroy GLFW window and terminate session
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}