#include "../Headers/windowCreation.h"



void BOG::windowCreation::destroy()
{
	glfwDestroyWindow(m_window);
	m_window = nullptr;
	m_winWidth = 0; m_winHeight = 0;
	m_winName = "";
}


GLFWwindow* BOG::windowCreation::getWindow()
{
	return m_window;
}

BOG::windowCreation::windowCreation(const char* name, int32_t width, int32_t height, bool primaryComp)
	:m_winName(name),m_winWidth(width),m_winHeight(height)
{
	if (primaryComp)
		m_window = glfwCreateWindow(m_winWidth, m_winHeight, m_winName, glfwGetPrimaryMonitor(), nullptr);
	else
		m_window = glfwCreateWindow(m_winWidth, m_winHeight, m_winName, nullptr, nullptr);
}

BOG::windowCreation::~windowCreation()
{
	if (m_window != nullptr)
		this->destroy();
	
		
}



