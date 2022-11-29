#include "../Headers/shaderProgram.h"
#include "../Headers/Camera.h"
#include "stb/stb_image.h"


Camera cam_1({ 0.f, 0.5f, 5.f });



int main()
{
	if (!appInitialize())
	{
		exit(1);
	}

	uint32_t VBO, VAO, IBO;

	ErrCheck(glGenVertexArrays(1, &VAO));
	ErrCheck(glGenBuffers(1, &VBO));
	ErrCheck(glGenBuffers(1, &IBO));

	ErrCheck(glBindVertexArray(VAO));
	ErrCheck(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	ErrCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO));

	ErrCheck(glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData) * 16, &block, GL_STATIC_DRAW));
	ErrCheck(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * 36, &blockIndex, GL_STATIC_DRAW));

	ErrCheck(glEnableVertexAttribArray(0));
	ErrCheck(glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(vertexData),&((vertexData*)0)->vertexPos));
	ErrCheck(glEnableVertexAttribArray(1));
	ErrCheck(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->vertexCol));
	ErrCheck(glEnableVertexAttribArray(2));
	ErrCheck(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vertexData), &((vertexData*)0)->texCoord));

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

	glm::mat4 model = glm::mat4(1.f);
	
	shaderProgram prog("D:\\Coding\\GameEngine\\sampLearningProj\\sampLearningProj\\additionalSrc\\shaders\\simple\\simpleObj.shader");

	float val[] = { 1.f,0.f,1.f };

	prog.bindProgram();
	int val1[] = { 0 };
	prog.setUniValueV<float*>("uColor", val, 3);
	prog.setUniValue<int>("texUnit", 0);
	prog.setUniValueM("transMat", &model[0].x, 4);
	prog.unbindProgram();

	fltPoint currFramTime = 0.f, preFramTime = 0.f, FramTimePeriod;

	uint32_t fCount = 0;


	glm::mat4 viewProjMat;
	
	while (!glfwWindowShouldClose((GLFWwindow*)mainWindow[0]))
	{

		
		currFramTime = glfwGetTime();
		FramTimePeriod = currFramTime - preFramTime;
		preFramTime = currFramTime;

		setColorBufer(1.f, .5f, .4f, 1.f);
		while (!glfwWindowShouldClose((GLFWwindow*)mainWindow[1]))
		{
			glfwMakeContextCurrent((GLFWwindow*)mainWindow[1]);
			glfwSwapBuffers((GLFWwindow*)mainWindow[1]);
			
			if (glfwGetKey((GLFWwindow*)mainWindow[1],GLFW_KEY_C) == GLFW_PRESS)
			{
				glfwSetWindowShouldClose((GLFWwindow*)mainWindow[1], true);
				glfwDestroyWindow((GLFWwindow*)mainWindow[1]);
				//glfwMakeContextCurrent((GLFWwindow*)mainWindow[0]);
			}
			glfwPollEvents();
		}
		

		glBindTextureUnit(0, texId);
		prog.bindProgram();
		
		prog.setUniValueM("viewProj", &viewProjMat[0].x, 4);
		ErrCheck(glBindVertexArray(VAO));
		ErrCheck(glDrawElements(GL_TRIANGLES, blockIndex.size(), GL_UNSIGNED_INT, 0));
		prog.unbindProgram();

		glfwSwapBuffers((GLFWwindow*)mainWindow[0]);
		cam_1.mvCam(FramTimePeriod);
		viewProjMat = cam_1.getViewProjMat();
		glfwPollEvents();
	}

	glfwDestroy();

	return 0;
}