#pragma once
#include <glm/glm.hpp>
#include <GL/glew.h>

class Light
{
	public:
		Light();
		Light(GLfloat red, GLfloat green, GLfloat blue, GLfloat aIntensity);

		void UseLight(GLfloat ambientIntensityLocation, GLfloat ambientColourLocation);


		~Light();
	private:
		glm::vec3 colour; // stores r g and b
		GLfloat ambientIntensity; // how intense this ambient light is going to be

};

