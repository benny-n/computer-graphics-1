#include "stdafx.h"
#include "Rasterizer.h"
#include "InitShader.h"

Rasterizer::Rasterizer() {
	mMiscProgram = InitShader("misc_vshader.glsl", "misc_fshader.glsl");
	mFlatProgram = InitShader("flat_vshader.glsl", "flat_fshader.glsl");
	mGouraudProgram = InitShader("gouraud_vshader.glsl", "gouraud_fshader.glsl");
	mPhongProgram = InitShader("phong_vshader.glsl", "phong_fshader.glsl");
	mToonProgram = InitShader("toon_vshader.glsl", "toon_fshader.glsl");
	mSilhouetteProgram = InitShader("silhouette_vshader.glsl", "silhouette_fshader.glsl");
	mNMProgram = InitShader("nm_vshader.glsl", "nm_fshader.glsl");
	mSkyboxProgram = InitShader("skybox_vshader.glsl", "skybox_fshader.glsl");
	mActiveProgram = mFlatProgram;
	glUseProgram(mActiveProgram);
	mShaderType = ShaderType::Flat;
}

GLuint Rasterizer::getMiscProgram() { return mMiscProgram; }
GLuint Rasterizer::getSilhouetteProgram() { return mSilhouetteProgram; }
GLuint Rasterizer::getNMProgram() { return mNMProgram; }
GLuint Rasterizer::getSkyboxProgram() { return mSkyboxProgram; }
GLuint Rasterizer::getActiveProgram() { return mActiveProgram; }
ShaderType Rasterizer::getShaderType() { return mShaderType; }

void Rasterizer::setActiveProgram(ShaderType shaderType) {
	mShaderType = shaderType;
	switch (shaderType) {
	case ShaderType::Flat:
		mActiveProgram = mFlatProgram;
		break;
	case ShaderType::Gouraud:
		mActiveProgram = mGouraudProgram;
		break;
	case ShaderType::Phong:
		mActiveProgram = mPhongProgram;
		break;
	case ShaderType::Toon:
		mActiveProgram = mToonProgram;
	}
	glUseProgram(mActiveProgram);
}
