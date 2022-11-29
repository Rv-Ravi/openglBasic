#include "../Headers/shaderProgram.h"

int32_t shaderProgram::m_shader[2] = {GL_VERTEX_SHADER,GL_FRAGMENT_SHADER};


shaderProgram::shaderProgram(const char* shaderPath) {
	this->m_progId = glCreateProgram();
	setProgram(shaderPath);
}
shaderProgram::~shaderProgram()
{
}

void shaderProgram::setProgram(const char* path) {

	std::ifstream shaderFile(path,std::ios::in);
	shaderProgram::m_shaderIndex index = shaderProgram::m_shaderIndex::NONE;
	std::stringstream shaderStream[2];
	if (shaderFile.is_open())
	{
		std::string line;
		while (std::getline(shaderFile,line)) {
			if (line != "\n" && line != "")
			{
				if (line.find("!#!shader") != std::string::npos)
				{
					if (line.find("vertex") != std::string::npos)
						index = m_shaderIndex::VERTEX;
					else
						index = m_shaderIndex::FRAGMENT;
				}
				else
					shaderStream[(int16_t)index] << line << "\n";
			}
		}
		for (int16_t i = 0; i < 2; i++)
			compileShader(shaderStream[i].str().c_str(), m_shader[i]);

		validateProgram();
	}

	shaderFile.close();


}
void shaderProgram::bindProgram() {
	ErrCheck(glUseProgram(m_progId));
}
void shaderProgram::unbindProgram() {
	ErrCheck(glUseProgram(0));
}

void shaderProgram::clearProgram()
{
	ErrCheck(glDeleteProgram(m_progId));
}

int32_t shaderProgram::getUlocation(const char* varName)
{
	auto val = m_uLocVal.find(varName);
	if (val == m_uLocVal.end())
	{
		ErrCheck(int tmpVal = glGetUniformLocation(m_progId, varName));
		m_uLocVal[varName] = tmpVal;
		return tmpVal;
	}
	return val->second;
}


void shaderProgram::compileShader(const char* shaderSrc, int32_t shaderIndex)
{
	uint32_t shaderVal = glCreateShader(shaderIndex);
	int32_t srcLen = std::strlen(shaderSrc);
	ErrCheck(glShaderSource(shaderVal, 1, &shaderSrc, & srcLen));

	ErrCheck(glCompileShader(shaderVal));
	int32_t cStat;
	ErrCheck(glGetShaderiv(shaderVal, GL_COMPILE_STATUS, &cStat));

	if (!cStat)
	{
		int32_t logLen;
		ErrCheck(glGetShaderiv(shaderVal, GL_INFO_LOG_LENGTH, &logLen));
		char* infoLog = new char[logLen + 1];
		ErrCheck(glGetShaderInfoLog(shaderVal, logLen, nullptr, infoLog));

		std::cerr << "The Error is : \n" << infoLog << std::endl;

		delete[logLen + 1] infoLog;
	}
	else
	{
		ErrCheck(glAttachShader(m_progId, shaderVal));
		ErrCheck(glDeleteShader(shaderVal));
	}

}

void shaderProgram::validateProgram() {

	ErrCheck(glLinkProgram(m_progId));
	ErrCheck(glValidateProgram(m_progId));

	int32_t lStat, vStat;
	ErrCheck(glGetProgramiv(m_progId, GL_LINK_STATUS, &lStat));
	ErrCheck(glGetProgramiv(m_progId, GL_VALIDATE_STATUS, &vStat));

	if (!lStat || !vStat)
	{
		int32_t logLen;
		ErrCheck(glGetProgramiv(m_progId, GL_INFO_LOG_LENGTH, &logLen));
		char* infoLog = new char[logLen + 1];
		ErrCheck(glGetProgramInfoLog(m_progId, logLen, nullptr, infoLog));

		std::cerr << "The Error is : " << (!lStat && !vStat ? "Link and Validate": (!lStat) ? "Link" : "Validate")
			<< "\n" << infoLog << std::endl;

		delete[logLen + 1] infoLog;
	}


}