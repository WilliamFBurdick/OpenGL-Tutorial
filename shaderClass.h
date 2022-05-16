#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

std::string get_file_contents(const char* filename);

class Shader
{
	public:
		//Reference ID to the program
		GLuint ID;
		//Construct shader program from 2 different shaders
		Shader(const char* vertexFile, const char* fragmentFile);

		//Activate shader program
		void Activate();
		//Delete shader program
		void Delete();

	private:
		void compileErrors(unsigned int shader, const char* type);
};

#endif
