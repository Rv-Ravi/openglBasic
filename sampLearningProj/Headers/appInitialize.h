#ifndef APP_INITIALIZE_H
#define APP_INITIALIZE_H


#include "windowCreation.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <array>
#include <vector>
#define SWIDTH 400
#define SHEIGHT 400



#define ErrCheck(func) while (glGetError());\
		func;\
		if(BOG::getErrorLog(#func,__FILE__,__LINE__)) __debugbreak()

namespace BOG {

	typedef float fltPoint;

	extern bool getErrorLog(const char* func, const char* file, int line);

	struct vertexData {
		glm::vec3 vertexPos;
		glm::vec3 vertexCol;
		glm::vec2 texCoord;
		glm::vec3 vertexNormal;
	};

	struct Materials {
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		BOG::fltPoint m_shininess;

		Materials(glm::vec3 amb = glm::vec3(0.2f),glm::vec3 dif = glm::vec3(0.8f),glm::vec3 spec = glm::vec3(1.f)
		,BOG::fltPoint shin = 20.f);
		~Materials();
	};


	struct TextureMate {
		uint32_t m_diffuse;
		uint32_t m_specular;
		BOG::fltPoint m_shininess;

		TextureMate(uint32_t dif = 0, uint32_t spec = 0, BOG::fltPoint shin = 20.f);
		~TextureMate();
	};

	struct LightEntity {
		glm::vec3 m_position, m_direction;

		glm::vec3 m_ambientIntensity = {0.2f,0.2f,0.2f}, m_diffuseIntensity = {0.8f,0.8f,0.8f}, m_specularIntensity = {1.f,1.f,1.f};

		BOG::fltPoint m_dist = 7.f, m_inAngle = 25.f, m_outAngle = 40.f;
	};

	extern std::array<uint16_t, GLFW_KEY_LAST> keyState;
	extern std::array<vertexData, 16> block;
	extern std::array<uint32_t, 36> blockIndex;
	extern std::array<vertexData, 4> square;
	extern std::array<uint32_t, 6> squareIndex;
	extern BOG::windowCreation* mainWindow;

	extern void setColorBufer(fltPoint x, fltPoint y, fltPoint z, fltPoint a);

	extern void keyEvents(GLFWwindow* window, int key, int scancode, int action, int mods);

	extern void glfwError(int id, const char* description);
	

	extern bool appInitialize();

}



#endif