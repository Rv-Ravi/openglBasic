#ifndef WINDOW_CREATION_H
#define WINDOW_CREATION_H

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace BOG
{

	class windowCreation
	{
	private:
		GLFWwindow* m_window = nullptr;
		const char* m_winName = "Application";
		int32_t m_winWidth, m_winHeight;
	public:
		
		windowCreation() = delete;
		windowCreation(const char* name, int32_t width, int32_t height, bool primaryComp = false);
		~windowCreation();

		void destroy();
		GLFWwindow* getWindow();

	private:
	};

}


#endif //WINDOW_CREATION_H