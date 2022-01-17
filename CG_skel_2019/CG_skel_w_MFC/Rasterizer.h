#pragma once

#include "gl/glew.h"

enum class ShaderType {
	Flat, Gouraud, Phong, Toon
};

class Rasterizer {
	GLuint mMiscProgram;
	GLuint mFlatProgram;
	GLuint mGouraudProgram;
	GLuint mPhongProgram;
	GLuint mToonProgram;
	GLuint mSilhouetteProgram;
	GLuint mNMProgram;
	GLuint mSkyboxProgram;
	GLuint mActiveProgram;
	ShaderType mShaderType;

public:
	Rasterizer();

	GLuint getMiscProgram();
	GLuint getSilhouetteProgram();
	GLuint getNMProgram();
	GLuint getSkyboxProgram();
	GLuint getActiveProgram();
	ShaderType getShaderType();
	void setActiveProgram(ShaderType shaderType);
};

typedef std::shared_ptr<Rasterizer> RasterizerPtr;


