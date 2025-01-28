#pragma once

#include "Debug.h"
#include <format>
#include <glad/glad.h>

#define GL_CALL(x) Renderer::ClearGLError(); x; { GLenum glCallResult = Renderer::CheckGLError(); ASSERT(glCallResult, GL_NO_ERROR, std::format("OpenGL Call error for ({})", #x)); }

namespace Renderer
{
	void Initialize();
	void Destroy();

	void ClearGLError();
	GLenum CheckGLError();

	void RenderFrame();
};