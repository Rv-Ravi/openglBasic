#include "../Headers/Camera.h"

glm::vec3 lerp(float alpha, const glm::vec3& a, const glm::vec3& b)
{
	return { (b.x - a.x) * alpha + a.x, (b.y - a.y) * alpha + a.y, (b.z - a.z) * alpha + a.z };
}


Camera::Camera()
	:m_camPos(0.f),m_camX(1.f, 0.f, 0.f),m_camY(0.f, 1.f, 0.f),m_camZ(0.f,0.f,1.f),m_camDir(-m_camZ),
	m_right(-10.f), m_left(10.f), m_top(10.f), m_bottom(-10.f),
	m_viewMat(1.f),m_projMat(1.f),m_viewProjMat(1.f),m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(90)
{
	calcViewMat();
	calcProjMat();
	calcViewProjMat();
}

Camera::Camera(const glm::vec3& camPos,bool isOrhto)
	:m_camPos(camPos), m_camX(1.f, 0.f, 0.f), m_camY(0.f, 1.f, 0.f), m_camZ(0.f, 0.f, 1.f), m_camDir(-m_camZ),
	m_right(-10.f), m_left(10.f), m_top(10.f), m_bottom(-10.f),
	m_viewMat(1.f), m_projMat(1.f), m_viewProjMat(1.f), m_camSpeed(0.f), m_zNear(0.01f), m_zFar(50.f), m_fov(90)
{
	this->isOrtho = isOrhto;
	calcViewMat();
	calcProjMat();
	calcViewProjMat();
}


Camera::~Camera()
{
}

void Camera::calcViewMat() {
	if (m_camZ != -m_camDir)
	{
		m_camZ = glm::normalize(m_camZ);
		m_camX = glm::normalize(glm::cross(m_camY, m_camZ));
		m_camY = glm::cross(m_camZ, m_camX);
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
		m_projMat = glm::ortho(m_left,m_right,m_bottom,m_top, m_zNear, m_zFar);
	else
		m_projMat = glm::perspective(glm::radians(m_fov), 16.f / 9.f, m_zNear, m_zFar);
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



void Camera::calcViewProjMat()
{
	m_viewProjMat = m_projMat * m_viewMat;
}

glm::vec3 Camera::getCamDir()
{
	return m_camDir;
}


void Camera::setFov()
{

}

void Camera::mvCam(fltPoint dtime)
{

	if (!isOrtho && keyState[GLFW_KEY_RIGHT_SHIFT] == 1 && keyState[GLFW_KEY_UP] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camZ);
	else if (!isOrtho && keyState[GLFW_KEY_RIGHT_SHIFT] == 1 && keyState[GLFW_KEY_DOWN] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camZ);
	else if (keyState[GLFW_KEY_UP] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camY);
	else if (keyState[GLFW_KEY_DOWN] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camY);
	else
		m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camZ);


	if (keyState[GLFW_KEY_RIGHT] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * m_camX);
	else if (keyState[GLFW_KEY_LEFT] == 1)
		m_camSpeed = lerp(dtime, m_camSpeed, m_camMaxVel * -m_camX);
	else
		m_camSpeed = lerp(dtime, m_camSpeed, 0.f * m_camX);


	m_camPos += m_camSpeed * dtime;

	calcViewMat();
	calcViewProjMat();

}
