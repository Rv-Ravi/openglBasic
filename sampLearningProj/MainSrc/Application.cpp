#include "../Headers/shaderProgram.h"
#include "../Headers/Camera.h"
#include "stb/stb_image.h"

uint32_t attachTexture(std::string path);

int main()
{
	if (!BOG::appInitialize())
	{
		exit(1);
	}
	
	BOG::Camera cam_1({ 0.f, 0.5f, 5.f });
	BOG::Camera cam_2({ 0.f, 5.5f, 5.f });


	glfwSetScrollCallback(BOG::mainWindow->getWindow(), [](GLFWwindow* window, double xPos, double yPos) {
			BOG::setFov(*BOG::currentCam, yPos);
		});

	uint32_t VBO, VAO, IBO;

	ErrCheck(glGenVertexArrays(1, &VAO));
	ErrCheck(glGenBuffers(1, &VBO));
	ErrCheck(glGenBuffers(1, &IBO));

	ErrCheck(glBindVertexArray(VAO));
	ErrCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	ErrCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));

	ErrCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(BOG::vertexData) * 16, &BOG::block, GL_STATIC_DRAW));
	ErrCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 36, &BOG::blockIndex, GL_STATIC_DRAW));

	ErrCheck(glEnableVertexAttribArray(0));
	ErrCheck(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(BOG::vertexData), &((BOG::vertexData*)0)->vertexPos));
	ErrCheck(glEnableVertexAttribArray(1));
	ErrCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(BOG::vertexData), &((BOG::vertexData*)0)->vertexCol));
	ErrCheck(glEnableVertexAttribArray(2));
	ErrCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(BOG::vertexData), &((BOG::vertexData*)0)->texCoord));
	ErrCheck(glEnableVertexAttribArray(3));
	ErrCheck(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(BOG::vertexData), &((BOG::vertexData*)0)->vertexNormal));

	ErrCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	ErrCheck(glBindVertexArray(0));
	ErrCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	
	BOG::TextureMate texture(attachTexture("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\Textures\\wallDiff.png"),
		attachTexture("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\Textures\\wallSpec.png"),
		2.f
	);

	glm::mat4 model = glm::mat4(1.f), model2 = glm::translate(glm::mat4(1.f),glm::vec3(0.f,5.f,0.f)) * 
		glm::scale(glm::mat4(1.f), glm::vec3(0.2f, 0.2f, 0.2f));

	glm::vec3 lightColor(1.f, 1.f, 1.f);
	//GOLD --> 0.24725	0.1995	0.0745	0.75164	0.60648	0.22648	0.628281	0.555802	0.366065	0.4
	BOG::Materials material(glm::vec3(0.24725,	0.1995,	0.0745),
		glm::vec3(0.75164,	0.60648	,0.22648),
		glm::vec3(0.628281,	0.555802,0.366065),
		102.4f
	);

	BOG::LightEntity light;

	shaderProgram prog("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\simple\\simpleObj.shader");
	shaderProgram prog1("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\simple\\simpLightShader.shader");
	shaderProgram prog2("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\simpleAmbientLit.shader");
	shaderProgram prog3("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\basicPhongLit.shader");
	shaderProgram prog4("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\materialPhongLit.shader");
	shaderProgram prog5("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\texturePhongLit.shader");
	shaderProgram prog6("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\dirLight.shader");
	shaderProgram prog7("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\Lighting\\pointLight.shader");
	float val[] = { 0.6f,0.3f,1.f };

	prog.bindProgram();
	prog.setUniValuefV("uColor", glm::vec3(0.6f, 0.3f, 1.f), 3);
	prog.setUniValue<int>("texUnit", 0);
	prog.setUniValueM("transMat", &model[0].x, 4);
	prog.unbindProgram();

	prog1.bindProgram();
	prog1.setUniValueM("transMat", &model2[0].x, 4);
	
	prog1.unbindProgram();

	prog2.bindProgram();
	prog2.setUniValuefV("uColor", glm::vec3(0.6f, 0.3f, 1.f), 3);
	prog2.setUniValue<int>("texUnit", 0);
	prog2.setUniValueM("transMat", &model[0].x, 4);
	prog2.unbindProgram();

	glm::mat3 normlMat = glm::mat3(1.f);

	prog3.bindProgram();
	prog3.setUniValuefV("lightColor", lightColor, 3);
	prog3.setUniValuefV("uColor", glm::vec3(0.6f, 0.3f, 1.f), 3);
	prog3.setUniValue<int>("texUnit", 0);
	prog3.setUniValueM("transMat", &model[0].x, 4);
	prog3.setUniValueM("normalMat", &normlMat[0].x, 3);
	prog3.unbindProgram();

	prog4.bindProgram();
	prog4.setUniValuefV("lightColor", lightColor, 3);
	prog4.setUniValueM("transMat", &model[0].x, 4);
	prog4.setUniValueM("normalMat", &normlMat[0].x, 3);
	prog4.setUniValuefV("material.m_ambient", material.m_ambient, 3);
	prog4.setUniValuefV("material.m_diffuse", material.m_diffuse, 3);
	prog4.setUniValuefV("material.m_specular", material.m_specular, 3);
	prog4.setUniValueV("material.m_shininess", material.m_shininess);
	prog4.unbindProgram();


	prog5.bindProgram();
	prog5.setUniValuefV("lightColor", lightColor, 3);
	prog5.setUniValueM("transMat", &model[0].x, 4);
	prog5.setUniValueM("normalMat", &normlMat[0].x, 3);
	ErrCheck(glBindTextureUnit(0,texture.m_diffuse));
	prog5.setUniValueV("material.m_diffuse", 0);
	ErrCheck(glBindTextureUnit(1, texture.m_specular));
	prog5.setUniValueV("material.m_specular", 1);
	prog5.setUniValueV("material.m_shininess", texture.m_shininess);
	prog5.unbindProgram();


	prog6.bindProgram();
	prog6.setUniValuefV("light.m_ambientIntensity", light.m_ambientIntensity, 3);
	prog6.setUniValuefV("light.m_diffuseIntensity", light.m_diffuseIntensity, 3);
	prog6.setUniValuefV("light.m_specularIntensity", light.m_specularIntensity, 3);
	prog6.setUniValueM("transMat", &model[0].x, 4);
	prog6.setUniValueM("normalMat", &normlMat[0].x, 3);
	ErrCheck(glBindTextureUnit(0, texture.m_diffuse));
	prog6.setUniValueV("material.m_diffuse", 0);
	ErrCheck(glBindTextureUnit(1, texture.m_specular));
	prog6.setUniValueV("material.m_specular", 1);
	prog6.setUniValueV("material.m_shininess", texture.m_shininess);
	prog6.unbindProgram();

	prog7.bindProgram();
	prog7.setUniValuefV("light.m_ambientIntensity", light.m_ambientIntensity, 3);
	prog7.setUniValuefV("light.m_diffuseIntensity", light.m_diffuseIntensity, 3);
	prog7.setUniValuefV("light.m_specularIntensity", light.m_specularIntensity, 3);
	prog7.setUniValueV("light.m_dist", 7.f);
	prog7.setUniValueM("transMat", &model[0].x, 4);
	prog7.setUniValueM("normalMat", &normlMat[0].x, 3);
	ErrCheck(glBindTextureUnit(0, texture.m_diffuse));
	prog7.setUniValueV("material.m_diffuse", 0);
	ErrCheck(glBindTextureUnit(1, texture.m_specular));
	prog7.setUniValueV("material.m_specular", 1);
	prog7.setUniValueV("material.m_shininess", texture.m_shininess);
	prog7.unbindProgram();
	BOG::fltPoint currFramTime = 0.f, preFramTime = 0.f, FramTimePeriod,chngVal = 42.f;

	uint32_t fCount = 0;


	glm::mat4 viewProjMat;
	

	while (!glfwWindowShouldClose(BOG::mainWindow->getWindow()))
	{

		
		currFramTime = glfwGetTime();
		FramTimePeriod = currFramTime - preFramTime;
		preFramTime = currFramTime;

		BOG::setColorBufer(0.25f,0.25f,0.25f,1.f);

		ErrCheck(glBindVertexArray(VAO));


		prog7.bindProgram();
		prog7.setUniValueM("viewProj", &viewProjMat[0].x, 4);
		prog7.setUniValuefV("light.m_position", BOG::currentCam->getPos(), 3);
		prog7.setUniValuefV("camPos", BOG::currentCam->getPos(), 3);

		ErrCheck(glDrawElements(GL_TRIANGLES, BOG::blockIndex.size(), GL_UNSIGNED_INT, 0));
		prog6.unbindProgram();

		ErrCheck(glBindVertexArray(0));
		glfwSwapBuffers(BOG::mainWindow->getWindow());
		BOG::currentCam->mvCam(FramTimePeriod);
		viewProjMat = BOG::currentCam->getViewProjMat();

		BOG::Camera::changeCam();

		glfwPollEvents();
	}

	delete BOG::mainWindow;
	return 0;
}

uint32_t attachTexture(std::string path)
{
	uint32_t texId;
	glGenTextures(1, &texId);
	glBindTexture(GL_TEXTURE_2D, texId);

	//float broderColor[] = { .5f,1.f,.78f,1.f };
	//glTextureParameterfv(texId, GL_TEXTURE_BORDER_COLOR, broderColor);

	glTextureParameteri(texId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(texId, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTextureParameteri(texId, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTextureParameteri(texId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int32_t iWidth, iHeight, iNoc;
	stbi_set_flip_vertically_on_load(1);
	unsigned char* imgData = stbi_load(path.c_str(), &iWidth, &iHeight, &iNoc, 0);

	if (imgData)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, iWidth, iHeight, 0, iNoc < 4 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, imgData);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cerr << "Unable to load the image file.\n";
	}
	stbi_image_free(imgData);

	glBindTexture(GL_TEXTURE_2D, 0);
	return texId;
}
