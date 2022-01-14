#pragma once

#include "gl/glew.h"

enum class ShaderType {
	Flat, Gouraud, Phong
};

class Rasterizer {
	GLuint mMiscProgram;
	GLuint mFlatProgram;
	GLuint mGouraudProgram;
	GLuint mPhongProgram;
	GLuint mNMProgram;
	GLuint mActiveProgram;
	ShaderType mShaderType;

public:
	Rasterizer();

	GLuint getMiscProgram();
	GLuint getNMProgram();
	GLuint getActiveProgram();
	ShaderType getShaderType();
	void setActiveProgram(ShaderType shaderType);
};

typedef std::shared_ptr<Rasterizer> RasterizerPtr;


