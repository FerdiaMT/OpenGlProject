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


GLuint VAO, VBO, IBO, shader , uniformModel;

bool direction = true;
float triOffset = 0.0f;
float triMaxOffset = 0.7f;
float triIncrement = 0.0005f;

float curAngle = 0.0f;

bool sizeDirection = true;
float curSize = 0.4;
float maxSize = 0.8;
float minSize = 0.1f;


//vertex shader

static const char* vShader = "											\n\
	#version 330														\n\
																		\n\
	layout (location = 0) in vec3 pos;									\n\
																		\n\
	uniform mat4 model;													\n\
	out vec4 vCol;													 \n\
	void main()															\n\
	{																	\n\
		gl_Position =  model *vec4(pos.x,pos.y,pos.z, 1.0);				\n\
		vCol = vec4(clamp(pos, 0.0f, 1.0f) , 1.0f);						 \n\
	}																	\n\
";

//clamp means if value is out of 0 and 1 put it in
static const char* fShader = "											\n\
	#version 330														\n\
																		\n\
	in vec4 vCol;														\n\
	out vec4 color;														\n\
	void main()															\n\
	{																	\n\
		color = vCol;													\n\
	}																	\n\
";

void moveFunction()
{

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

	curAngle += 0.01f;
	if (curAngle >= 360)
	{
		curAngle -= 360;
	}

	if (sizeDirection)
	{
		curSize += 0.0001f;
	}
	else
	{
		curSize -= 0.0001f;
	}

	if (curSize >= maxSize || curSize <= minSize) {
		sizeDirection = !sizeDirection;
	}
}

void createTriangle() 
{

	unsigned int indices[] = {
	0,3,1,
	1,3,2,
	2,3,0,
	0,1,2
	};

	GLfloat vertices[] = {
		-1.0f, -1.0f, 0.0f,
		0.0f , -1.0f, 1.0f,
		1.0f, -1.0f, 0.0f,	
		0.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO); // 1 is the amount of arrays we wanna make
	glBindVertexArray(VAO);

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // binds the current VBO(stores the little data) to the big box
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	// shader ref , size per veretx (xyz) , type, normalized ? , stride,  wjhere to start
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


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

	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "FerdGraphics Engine Test V1.4", NULL, NULL);
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

	glEnable(GL_DEPTH_TEST);
	//setup viewport size

	glViewport(0, 0, bufferWidth, bufferHeight);

	createTriangle();
	compileShaders();
	//set up the main loop, loop until window closed
	while (!glfwWindowShouldClose(mainWindow)) 
	{
		// GET AND HANDLE USER INPUT EVENTS
		glfwPollEvents(); // has anything happened

		moveFunction();

		glClearColor(0.0f, 0.0f, 0.0f,1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shader);

		glm::mat4 model(1.0f); // 1 0 0 0
							   // 0 1 0 0
							   // 0 0 1 0 
							   // 0 0 0 1
		
		model = glm::rotate(model, curAngle * toRadians, glm::vec3(1.0f, 1.0f,0.0f));
		//model = glm::translate(model, glm::vec3(triOffset, 0.0f, 0.0f));
		
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IBO);


		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}