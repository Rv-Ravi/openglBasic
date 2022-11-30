#include "../Headers/Camera.h"

namespace BOG {

	std::vector<BOG::Camera*> BOG::Camera::ms_camList;
	glm::vec3 lerp(float alpha, const glm::vec3& a, const glm::vec3& b)
	{
		return { (b.x - a.x) * alpha + a.x, (b.y - a.y) * alpha + a.y, (b.z - a.z) * alpha + a.z };
	}


	Camera::Camera()
		:m_camPos(0.f), m_camX(1.f, 0.f, 0.f), m_camY(0.f, 1.f, 0.f), m_camZ(0.f, 0.f, 1.f), m_camDir(-m_camZ),
		m_right(10.f), m_left(-10.f), m_top(10.f), m_bottom(-10.f), m_pitch(0.f), m_yaw(0.f), m_lastX(0.f), m_lastY(0.f),
		m_viewMat(1.f), m_projMat(1.f), m_viewProjMat(1.f), m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(45)
	{
		if (ms_camList.begin() == ms_camList.end())
		{
			ms_camList.reserve(5);
			BOG::currentCam = this;
		}
			
		if (ms_camList.size() < ms_camList.capacity()) ms_camList.emplace_back(this);
		calcViewMat();
		calcProjMat();
		calcViewProjMat();

	}

	Camera::Camera(const glm::vec3& camPos, bool isOrhto)
		:m_camPos(camPos), m_camX(1.f, 0.f, 0.f), m_camY(0.f, 1.f, 0.f), m_camZ(0.f, 0.f, 1.f), m_camDir(-m_camZ),
		m_right(10.f), m_left(-10.f), m_top(10.f), m_bottom(-10.f), m_pitch(0.f), m_yaw(0.f), m_lastX(0.f), m_lastY(0.f),
		m_viewMat(1.f), m_projMat(1.f), m_viewProjMat(1.f), m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(45)
	{
		if (ms_camList.begin() == ms_camList.end())
		{
			ms_camList.reserve(5);
			BOG::currentCam = this;
		}
			
		if (ms_camList.size() < ms_camList.capacity()) ms_camList.emplace_back(this);
		this->isOrtho = isOrhto;
		calcViewMat();
		calcProjMat();
		calcViewProjMat();
	}


	Camera::~Camera()
	{
		std::vector<BOG::Camera*>::iterator ite = std::find(ms_camList.begin(), ms_camList.end(), this);
		if (ite != ms_camList.end())
		{
			*ite = nullptr;
			ms_camList.erase(ite);
		}
	}

	void Camera::calcViewMat() {
		if (m_camZ != -m_camDir)
		{
			m_camZ = glm::normalize(m_camZ);
			m_camX = glm::normalize(glm::cross(m_camY, m_camZ));
			m_camY = glm::cross(m_camZ, m_camX);
			m_camDir = -m_camZ;
		}

		m_viewMat[0][0] = m_camX.x; m_viewMat[1][0] = m_camX.y; m_viewMat[2][0] = m_camX.z;
		m_viewMat[0][1] = m_camY.x; m_viewMat[1][1] = m_camY.y; m_viewMat[2][1] = m_camY.z;
		m_viewMat[0][2] = m_camZ.x; m_viewMat[1][2] = m_camZ.y; m_viewMat[2][2] = m_camZ.z;
		m_viewMat[3][0] = -glm::dot(m_camX, m_camPos);
		m_viewMat[3][1] = -glm::dot(m_camY, m_camPos);
		m_viewMat[3][2] = -glm::dot(m_camZ, m_camPos);
	}

	void Camera::calcProjMat() {
		if (isOrtho)
			m_projMat = glm::ortho(m_left,m_right,m_bottom,m_top,0.f,5.f);
		else
			m_projMat = glm::perspective(glm::radians(m_fov), 16.f / 9.f, m_zNear, m_zFar);
	}

	void Camera::camChngProj() {
		if (BOG::keyState[GLFW_KEY_P] == 1 && BOG::keyState[GLFW_KEY_1] == 1)
		{
			if (isOrtho == false)
			{
				isOrtho = true;
				calcProjMat();
			}
		}
		else if (BOG::keyState[GLFW_KEY_P] == 1 && BOG::keyState[GLFW_KEY_2] == 1)
		{
			if (isOrtho == true)
			{
				isOrtho = false;
				calcProjMat();
			}
		}
	}
	void Camera::setPos(const glm::vec3& pos)
	{
		m_camPos.x = pos.x;
		m_camPos.y = pos.y;
		m_camPos.z = pos.z;
	}

	glm::vec3 Camera::getPos()
	{
		return m_camPos;
	}

	void Camera::changeCam() {
		if (BOG::keyState[GLFW_KEY_C] == 1 && BOG::keyState[GLFW_KEY_1] == 1)
		{
			BOG::currentCam = (ms_camList[0])? ms_camList[0] : BOG::currentCam;
		}
		else if (BOG::keyState[GLFW_KEY_C] == 1 && BOG::keyState[GLFW_KEY_2] == 1)
		{
			BOG::currentCam = (ms_camList[1]) ? ms_camList[1] : BOG::currentCam;
		}
		else if (BOG::keyState[GLFW_KEY_C] == 1 && BOG::keyState[GLFW_KEY_3] == 1)
		{
			BOG::currentCam = (ms_camList[2]) ? ms_camList[2] : BOG::currentCam;
		}
		else if (BOG::keyState[GLFW_KEY_C] == 1 && BOG::keyState[GLFW_KEY_4] == 1)
		{
			BOG::currentCam = (ms_camList[3]) ? ms_camList[3] : BOG::currentCam;
		}
		else if (BOG::keyState[GLFW_KEY_C] == 1 && BOG::keyState[GLFW_KEY_5] == 1)
		{
			BOG::currentCam = (ms_camList[4]) ? ms_camList[4] : BOG::currentCam;
		}
	}

	void Camera::calcViewProjMat()
	{
		m_viewProjMat = m_projMat * m_viewMat;
	}

	glm::vec3 Camera::getCamDir()
	{
		return m_camDir;
	}

	void Camera::rotateCam(BOG::fltPoint dtime)
	{
		if (glfwGetMouseButton(BOG::mainWindow->getWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS && !this->isOrtho)
		{
			double posX, posY;
			glfwGetCursorPos(BOG::mainWindow->getWindow(), &posX, &posY);

			if (!isMouInit)
			{
				m_lastX = posX;
				m_lastY = posY;
				isMouInit = true;
			}

			m_yaw += (m_lastX - posX) * dtime * 4.f;
			m_pitch += (posY - m_lastY) * dtime * 4.f;

			m_lastY = posY;
			m_lastX = posX;

			m_camZ.x = cos(glm::radians(m_pitch)) * sin(glm::radians(m_yaw));
			m_camZ.y = sin(glm::radians(m_pitch));
			m_camZ.z = cos(glm::radians(m_pitch)) * cos(glm::radians(m_yaw));

		}
		else {
			isMouInit = false;
		}
	}

	void Camera::mvCam(BOG::fltPoint dtime)
	{
		rotateCam(dtime);
		if (BOG::keyState[GLFW_KEY_RIGHT_SHIFT] == 1 && BOG::keyState[GLFW_KEY_UP] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camY);
		else if (BOG::keyState[GLFW_KEY_RIGHT_SHIFT] == 1 && BOG::keyState[GLFW_KEY_DOWN] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camY);
		else if (!isOrtho && BOG::keyState[GLFW_KEY_UP] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camZ);
		else if (!isOrtho && BOG::keyState[GLFW_KEY_DOWN] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camZ);
		else
			m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camZ);


		if (BOG::keyState[GLFW_KEY_RIGHT] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camX);
		else if (BOG::keyState[GLFW_KEY_LEFT] == 1)
			m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camX);
		else
			m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camX);


		m_camPos += m_camSpeed * dtime;
		camChngProj();
		calcViewMat();
		calcViewProjMat();

	}
	BOG::Camera* BOG::currentCam = nullptr;
}