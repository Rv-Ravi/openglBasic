#ifndef CAMERA_H
#define CAMERA_H

#include "appInitialize.h"

namespace BOG {

	class Camera
	{
	private:
		glm::vec3 m_camPos, m_camX, m_camY, m_camZ, m_camDir, m_camMaxVel = glm::vec3(6.3f), m_camSpeed;
		glm::mat4 m_viewMat, m_projMat, m_viewProjMat;

		static std::vector<BOG::Camera*> ms_camList;

		BOG::fltPoint m_zNear, m_zFar, m_fov, m_right, m_left, m_top, m_bottom, m_pitch,m_yaw,m_lastX,m_lastY;
		bool isOrtho = false,isMouInit = false;

	public:
		
		//Constructors
		Camera();
		Camera(const glm::vec3& camPos, bool isOrtho = true);

		~Camera();

		//setters and getters
		void setPos(const glm::vec3& pos);
		glm::vec3 getPos();

		inline glm::mat4 getViewMat() { return m_viewMat; }
		inline glm::mat4 getProjMat() { return m_projMat; }
		inline glm::mat4 getViewProjMat() { return m_viewProjMat; }

		glm::vec3 getCamDir();

		static void changeCam();
		void camChngProj();

		inline void setOrtho(bool val) { isOrtho = val; }
		inline bool getOrtho() { return isOrtho; }
		//methods
		void calcViewMat();
		void calcProjMat();
		void calcViewProjMat();

		void rotateCam(BOG::fltPoint dtime);

		friend void setFov(BOG::Camera& camera,BOG::fltPoint yPos);
		void mvCam(BOG::fltPoint dtime);

	};

	static void setFov(BOG::Camera& camera, BOG::fltPoint yPos)
	{
		camera.m_fov -= yPos;
		if (camera.m_fov > 60.f)
		{
			camera.m_fov = 60.f;
		}
		else if (camera.m_fov < 30.f)
		{
			camera.m_fov = 30.f;
		}
		else {
			camera.calcProjMat();
			camera.calcViewProjMat();
		}
		//camera.m_fov = (camera.m_fov > 60.f) ? 60.f : (camera.m_fov < 30.f) ? 30.f : camera.m_fov;	
	}


	extern BOG::Camera *currentCam;
}
#endif //CAMERA_H

