#ifndef APP_INITIALIZE_H
#define APP_INITIALIZE_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <vector>

#define SWIDTH 400
#define SHEIGHT 400


struct vertexData {
	glm::vec3 vertexPos;
	glm::vec3 vertexCol;
	glm::vec2 texCoord;
	glm::vec3 vertexNormal;
};



static bool getErrorLog(const char* func, const char* file, int line)
{
	while (uint32_t error = glGetError())
	{
		std::cerr << "OpenGL Error: (" << error << "):" << file << ":" << line << ":" << func << std::endl;
		return true;
	}
	return false;
}

#define ErrCheck(func) while (glGetError());\
	func;\
	if(getErrorLog(#func,__FILE__,__LINE__)) __debugbreak()



typedef float fltPoint;

static const GLFWwindow* mainWindow[5];

extern std::array<uint16_t, GLFW_KEY_LAST> keyState;
extern std::array<vertexData,16> block;
extern std::array<uint32_t,36> blockIndex;
extern std::array<vertexData,4> square;
extern std::array<uint32_t,6> squareIndex;


static void setColorBufer(fltPoint x, fltPoint y, fltPoint z, fltPoint a) {
	ErrCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	ErrCheck(glClearDepth(1.0f));
	ErrCheck(glClearColor(x, y, z, a));
}



static void glfwDestroy()
{
	for (const GLFWwindow* window : mainWindow)
	{
		if (!window)
		{
			break;
		}
		glfwDestroyWindow((GLFWwindow*)window);
	}
	
	glfwTerminate();
}


inline void keyEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS || action == GLFW_RELEASE)
		keyState[key] = action;
}


static void glfwError(int id, const char* description)
{
	std::cout << "(Error ID: " << id << ") = " << description << std::endl;
}


static bool appInitialize()
{
	glfwSetErrorCallback(glfwError);
	if (!glfwInit())
	{
		std::cerr << "Unable to initialize GLFW and its window context. Please check the code.\n";
		return false;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	mainWindow[0] = glfwCreateWindow(SWIDTH, SHEIGHT, "APPLICATION", nullptr, nullptr);
	if (!mainWindow[0])
	{
		std::cerr << "Unable to create GLFW window.\n";
		glfwTerminate();
		return false;
	}
	glfwSetKeyCallback((GLFWwindow*)mainWindow[0], keyEvents);
	
	glfwMakeContextCurrent((GLFWwindow*)mainWindow[0]);
	glfwSwapInterval(1);

	mainWindow[1] = glfwCreateWindow(100, 100, "APPLICATION - 1", nullptr, nullptr);
	if (!mainWindow[1])
	{
		std::cerr << "Unable to create GLFW window.\n";
		glfwTerminate();
		return false;
	}

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Unable to initialize GLAD OPENGL. please check the code.\n";
		glfwDestroyWindow((GLFWwindow*)mainWindow[0]);
		glfwTerminate();
		return false;
	}

	glfwSetFramebufferSizeCallback((GLFWwindow*)mainWindow[0], [](GLFWwindow* window, int width, int height) {
		ErrCheck(glViewport(0, 0, width, height));
		});
	ErrCheck(glEnable(GL_DEPTH_TEST));
	setColorBufer(1.f, .5f, .4f, 1.f);

	return true;
}

#endif // !APP_INITIALIZE_H
