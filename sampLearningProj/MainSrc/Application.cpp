#include "../Headers/shaderProgram.h"
#include "../Headers/Camera.h"
#include "stb/stb_image.h"



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

	ErrCheck(glBindBuffer(GL_ARRAY_BUFFER, 0));
	ErrCheck(glBindVertexArray(0));
	ErrCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


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
	unsigned char* imgData = stbi_load("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\Textures\\5052652.png", &iWidth, &iHeight, &iNoc, 0);

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

	glm::mat4 model = glm::mat4(1.f), model2 = glm::translate(glm::mat4(1.f),glm::vec3(0.f,5.f,0.f)) * 
		glm::scale(glm::mat4(1.f), glm::vec3(0.2f, 0.2f, 0.2f));

	glm::vec3 lightColor(0.f, 0.f, 0.f);

	shaderProgram prog("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\simple\\simpleObj.shader");
	shaderProgram prog1("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\simple\\simpLightShader.shader");
	float val[] = { 1.f,0.f,1.f };

	prog.bindProgram();
	prog.setUniValueV<float*>("uColor", val, 3);
	prog.setUniValue<int>("texUnit", 0);
	prog.setUniValueM("transMat", &model[0].x, 4);
	prog.unbindProgram();

	prog1.bindProgram();
	prog1.setUniValueM("transMat", &model2[0].x, 4);
	
	prog1.unbindProgram();

	BOG::fltPoint currFramTime = 0.f, preFramTime = 0.f, FramTimePeriod,chngVal = 42.f;

	uint32_t fCount = 0;


	glm::mat4 viewProjMat;
	

	while (!glfwWindowShouldClose(BOG::mainWindow->getWindow()))
	{

		
		currFramTime = glfwGetTime();
		FramTimePeriod = currFramTime - preFramTime;
		preFramTime = currFramTime;

		BOG::setColorBufer(lightColor.x, lightColor.y, lightColor.z,1.f);

		glBindTextureUnit(0, texId);
		ErrCheck(glBindVertexArray(VAO));
		prog.bindProgram();

		prog.setUniValueM("viewProj", &viewProjMat[0].x, 4);
		
		ErrCheck(glDrawElements(GL_TRIANGLES, BOG::blockIndex.size(), GL_UNSIGNED_INT, 0));
		prog.unbindProgram();

		prog1.bindProgram();
		prog1.setUniValuefV("lightColor", lightColor, 3);
		prog1.setUniValueM("viewProj", &viewProjMat[0].x, 4);

		ErrCheck(glDrawElements(GL_TRIANGLES, BOG::blockIndex.size(), GL_UNSIGNED_INT, 0));
		prog1.unbindProgram();

		ErrCheck(glBindVertexArray(0));
		glfwSwapBuffers(BOG::mainWindow->getWindow());
		BOG::currentCam->mvCam(FramTimePeriod);
		viewProjMat = BOG::currentCam->getViewProjMat();

		BOG::Camera::changeCam();

		lightColor = glm::vec3(abs(sin(chngVal + 9.f)), abs(sin(chngVal - 12.f)), abs(sin(chngVal - 5.f)));
		chngVal += FramTimePeriod;

		glfwPollEvents();
	}

	delete BOG::mainWindow;
	return 0;
}