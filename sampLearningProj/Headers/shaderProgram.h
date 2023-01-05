#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include "appInitialize.h"
#include <fstream>
#include <sstream>
#include <unordered_map>




class shaderProgram {

private:
	uint32_t m_progId;
	enum class m_shaderIndex : int16_t
	{
		NONE=-1,VERTEX=0,FRAGMENT=1
	};
	static int32_t m_shader[2];

	std::unordered_map<std::string, int32_t> m_uLocVal;

public:
	shaderProgram(const char* shaderPath);
	~shaderProgram();
	void setProgram(const char* path);
	void bindProgram();
	void unbindProgram();
	void clearProgram();

	int32_t getUlocation(const char* varName);
	
	template<typename varType>
	void setUniValueV(const char* varName,const varType value)
	{
		int32_t tmpVal = getUlocation(varName);

		if (typeid(varType) == typeid(float))
		{
			ErrCheck(glUniform1f(tmpVal, value));
		}

		else if (typeid(varType) == typeid(int32_t))
		{
			ErrCheck(glUniform1i(tmpVal, value));
		}
		
	}

	template<typename varType>
	void setUniValuefV(const char* varName, const varType& value, uint16_t size)
	{
		int32_t tmpVal = getUlocation(varName);
		switch (size)
		{
		case 1: ErrCheck(glUniform1fv(tmpVal, 1, &value.x)); break;
		case 2: ErrCheck(glUniform2fv(tmpVal, 1, &value.x)); break;
		case 3: ErrCheck(glUniform3fv(tmpVal, 1, &value.x)); break;
		case 4: ErrCheck(glUniform4fv(tmpVal, 1, &value.x)); break;
		default: printf("No such value is found for setting int* type as uniform."); break;
		}
	}

	template<typename varType>
	void setUniValueiV(const char* varName, const varType& value, uint16_t size)
	{
		int32_t tmpVal = getUlocation(varName);
		switch (size)
		{
		case 1: ErrCheck(glUniform1iv(tmpVal, 1, &value.x)); break;
		case 2: ErrCheck(glUniform2iv(tmpVal, 1, &value.x)); break;
		case 3: ErrCheck(glUniform3iv(tmpVal, 1, &value.x)); break;
		case 4: ErrCheck(glUniform4iv(tmpVal, 1, &value.x)); break;
		default: printf("No such value is found for setting int* type as uniform."); break;
		}
	}

	void setUniValueM(const char* varName, const float* value, uint16_t size)
	{
		int32_t tmpVal = getUlocation(varName);
		switch (size)
		{
		case 2: ErrCheck(glUniformMatrix2fv(tmpVal,1,GL_FALSE,value)); break;
		case 3: ErrCheck(glUniformMatrix3fv(tmpVal, 1, GL_FALSE, value)); break;
		case 4: ErrCheck(glUniformMatrix4fv(tmpVal, 1, GL_FALSE, value)); break;
		default: printf("No such value is found for setting float* type as uniform."); break;
		}
	}
	
	template<typename varType>
	void setUniValue(const char* varName, const varType value)
	{
		int32_t tmpVal = getUlocation(varName);

		if (typeid(varType) == typeid(float))
		{
			ErrCheck(glUniform1f(tmpVal, value));
		}

		else if (typeid(varType) == typeid(int32_t*))
		{
			ErrCheck(glUniform1i(tmpVal, value));
		}
	}

private:
	void compileShader(const char* shaderSrc, int32_t shaderIndex);
	void validateProgram();

};



#endif //SHADER_PROGRAM_H