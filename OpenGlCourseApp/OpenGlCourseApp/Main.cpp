#include <stdio.h>
#include <string.h>
#include <cmath>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>



// this is our window dimensions
const GLint WIDTH = 800;
const GLint HEIGHT = 600;
const float toRadians = 3.14159265f / 180.0f;


GLuint VAO, VBO, shader , uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;




//vertex shader

static const char* vShader = "											\n\
	#version 330														\n\
																		\n\
	layout (location = 0) in vec3 pos;									\n\
																		\n\
	uniform mat4 model;												\n\
	void main()															\n\
	{																	\n\
		gl_Position =  model *vec4(pos.x * 0.4,pos.y * 0.4,pos.z * 0.4, 1.0);	\n\
	}																	\n\
";

static const char* fShader = "											\n\
	#version 330														\n\
																		\n\
	out vec4 color;														\n\
	void main()															\n\
	{																	\n\
		color = vec4(1.0,0.0,0.0,1.0);									\n\
	}																	\n\
";



void createTriangle() 
{
	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO); // 1 is the amount of arrays we wanna make
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds the current VBO(stores the little data) to the big box

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// shader ref , size per veretx (xyz) , type, normalized ? , stride,  wjhere to start
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);


	// vertex array, this contains the vbo
	// vertex buffer , this contains some data
	//bufferdata , how we insert into the buffer
	// create a vertex pointer, allows us to navigate through the array of vertices
}

void addShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) 
{
	GLuint theShader = glCreateShader(shaderType);
	const GLchar* theCode[1];
	theCode[0] = shaderCode;
	
	GLint codeLength[1];
	codeLength[0] = strlen(shaderCode);

	glShaderSource(theShader, 1, theCode, codeLength);
	glCompileShader(theShader);


	GLint result = 0;
	GLchar eLog[1024] = { 0 };


	glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
		printf("Error compiling the the program %s ",  eLog);
		return;
	};
	glAttachShader(theProgram, theShader);

}

void compileShaders() 
{
	shader = glCreateProgram(); // creathes the program and gives shader the id
	if (!shader) 
	{
		printf("error creating the shader program");
		return;
	}

	addShader(shader, vShader, GL_VERTEX_SHADER);
	addShader(shader, fShader, GL_FRAGMENT_SHADER);

	GLint result = 0;
	GLchar eLog[1024] = { 0 };

	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error linking the program %s" , eLog);
		return;
	};

	glValidateProgram(shader);
	glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
	if (!result)
	{
		glGetProgramInfoLog(shader, sizeof(eLog), NULL, eLog);
		printf("Error validating the program %s", eLog);
		return;
	}

	uniformModel = glGetUniformLocation(shader, "model");


}

int main() 
{

	if (!glfwInit()) // if its a fail
	{ 
		printf("GLFW INIT FAIL");
		glfwTerminate();
		return 1;
	}

	// set up our glfw window properties

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "TEST WINDOW", NULL, NULL);
	if (!mainWindow) 
	{
		printf("GLFW window creation failed!");
		glfwTerminate();
		return -1;
	}

	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);
	glfwMakeContextCurrent(mainWindow);

	glewExperimental = GL_TRUE;


	if (glewInit() != GLEW_OK) 
	{
		printf("GLEW INIT FAILED");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//setup viewport size

	glViewport(0, 0, bufferWidth, bufferHeight);

	createTriangle();
	compileShaders();
	//set up the main loop, loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		// GET AND HANDLE USER INPUT EVENTS
		glfwPollEvents(); // has anything happened

		if (direction) // if direction is true - > going to the right
		{
			triOffset += triIncrement;
		}
		else
		{
			triOffset -= triIncrement;
		}

		if (abs(triOffset) >= triMaxOffset)
		{
			direction = !direction;
		}

		curAngle += 0.001f;
		if (curAngle >= 360)
		{
			curAngle -= 360;
		}

		glClearColor(0.0f, 0.0f, 0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f); // 1 0 0 0
							   // 0 1 0 0
							   // 0 0 1 0 
							   // 0 0 0 1
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		


		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));


		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glBindVertexArray(0);

		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}