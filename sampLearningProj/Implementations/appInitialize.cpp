#include "../Headers/appInitialize.h"


namespace BOG {


	bool getErrorLog(const char* func, const char* file, int line)
	{
		while (uint32_t error = glGetError())
		{
			std::cerr << "OpenGL Error: (" << error << "):" << file << ":" << line << ":" << func << std::endl;
			return true;
		}
		return false;
	}


	std::array<uint32_t, 36> blockIndex =
	{
		//TOP
		0,1,2,
		0,2,3,
		//BOTTOM
		4,5,6,
		4,6,7,
		//FRONT
		8,9,10,
		8,10,11,
		//BACK
		12,13,14,
		12,14,15,
		//RIGHT
		9,10,13,
		10,14,13,
		//LEFT
		8,11,12,
		11,15,12
	};
	std::array<BOG::vertexData, 16> block =
	{   //POSITION	                   //COLOR					  //TEX-CO					//NORMAL
		//TOP
		BOG::vertexData(glm::vec3(-1.0f,1.0f,1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),     glm::vec3(-0.33f,0.33f,0.33f)),
		{glm::vec3(1.0f,1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.9f),	    glm::vec3(0.33f,0.33f,0.33f)},
		{glm::vec3(1.0f,1.0f,-1.0f),	  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.9f),	    glm::vec3(0.33f,0.33f,-0.33f)},
		{glm::vec3(-1.0f,1.0f,-1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(-0.33f,0.33f,-0.33f)},

		//BOTTOM
		{glm::vec3(-1.0f,-1.0f,1.0f),  glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.0f)},
		{glm::vec3(1.0f,-1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.0f)},
		{glm::vec3(1.0f,-1.0f,-1.0f),  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.0f)},
		{glm::vec3(-1.0f,-1.0f,-1.0f), glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.0f)},

		//FRONT
		{glm::vec3(-1.0f,1.0f,1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(0.0f,1.0f),     glm::vec3(-0.33f,0.33f,0.33f)},
		{glm::vec3(1.0f,1.0f,1.0f),	  glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(0.33f,0.33f,0.33f)},
		{glm::vec3(1.0f,-1.0f,1.0f),	  glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.33f,-0.33f,0.33f)},
		{glm::vec3(-1.0f,-1.0f,1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(-0.33f,-0.33f,0.33f)},

		//BACK
		{glm::vec3(-1.0f,1.0f,-1.0f),   glm::vec3(1.0f,0.0f,0.0f),  glm::vec2(1.0f,1.0f),        glm::vec3(-0.33f,0.33f,-0.33f)},
		{glm::vec3(1.0f,1.0f,-1.0f),	   glm::vec3(0.0f,1.0f,0.0f),  glm::vec2(0.0f,1.0f),	    glm::vec3(0.33f,0.33f,-0.33f)},
		{glm::vec3(1.0f,-1.0f,-1.0f),   glm::vec3(0.0f,0.0f,1.0f),  glm::vec2(0.0f,0.0f),	    glm::vec3(0.33f,-0.33f,-0.33f)},
		{glm::vec3(-1.0f,-1.0f,-1.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(-0.33f,-0.33f,-0.33f)}

	};



	std::array<BOG::vertexData, 4> square = {
		BOG::vertexData(glm::vec3(-1.0f,-1.0f,0.0f),   glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,0.0f),     glm::vec3(0.f,0.f,1.f)),
		{glm::vec3(1.0f,-1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,0.0f),	    glm::vec3(0.f,0.f,1.f)},
		{glm::vec3(1.0f,1.0f,0.0f),	  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(1.0f,1.0f),	    glm::vec3(0.f,0.f,1.f)},
		{glm::vec3(-1.0f,1.0f,0.0f),  glm::vec3(1.0f,1.0f,1.0f),  glm::vec2(0.0f,1.0f),	    glm::vec3(0.f,0.f,1.f)},

	};

	std::array<uint32_t, 6> squareIndex = {
			0,1,2,
			0,2,3
	};


	std::array<uint16_t, GLFW_KEY_LAST> keyState = { 0 };
	windowCreation* mainWindow = nullptr;

	void setColorBufer(fltPoint x, fltPoint y, fltPoint z, fltPoint a) {
		ErrCheck(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		ErrCheck(glClearDepth(1.0f));
		ErrCheck(glClearColor(x, y, z, a));
	}

	void keyEvents(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (action == GLFW_PRESS || action == GLFW_RELEASE)
			keyState[key] = action;
	}

	void glfwError(int id, const char* description)
	{
		std::cout << "(Error ID: " << id << ") = " << description << std::endl;
	}

	bool appInitialize()
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

		mainWindow = new BOG::windowCreation("Application", SWIDTH, SHEIGHT);

		if (!mainWindow->getWindow())
		{
			std::cerr << "Unable to create GLFW window.\n";
			glfwTerminate();
			return false;
		}

		glfwSetKeyCallback(BOG::mainWindow->getWindow(), keyEvents);
		glfwMakeContextCurrent(BOG::mainWindow->getWindow());
		glfwSwapInterval(1);


		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cerr << "Unable to initialize GLAD OPENGL. please check the code.\n";
			glfwDestroyWindow(BOG::mainWindow->getWindow());
			glfwTerminate();
			return false;
		}

		glfwSetFramebufferSizeCallback(BOG::mainWindow->getWindow(), [](GLFWwindow* window, int width, int height) {
			ErrCheck(glViewport(0, 0, width, height));
			});
		ErrCheck(glEnable(GL_DEPTH_TEST));


		setColorBufer(1.f, .5f, .4f, 1.f);
		return true;
	}
	Materials::Materials(glm::vec3 amb, glm::vec3 dif , glm::vec3 spec
		, BOG::fltPoint shin)
	{
		m_ambient = amb;
		m_diffuse = dif;
		m_specular = spec;
		m_shininess = shin;
	}
	Materials::~Materials()
	{
	}
	TextureMate::TextureMate(uint32_t dif, uint32_t spec, BOG::fltPoint shin)
	{
		m_diffuse = dif;
		m_specular = spec;
		m_shininess = shin;
	}
	TextureMate::~TextureMate()
	{
	}
}