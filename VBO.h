#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <glad/glad.h>

class VBO
{
	public:
		//Reference ID of the Vertex Buffer Object
		GLuint ID;
		//Constructor that generates Vertex Buffer Object and links it to vertices
		VBO(GLfloat* vertices, GLsizeiptr size);

		//Binds VBO
		void Bind();
		//Unbinds VBO
		void Unbind();
		//Deletes VBO
		void Delete();
};

#endif