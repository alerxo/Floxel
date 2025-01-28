#include "Rendering/Renderer.h"

#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Resources.h"

#include <SDL3/SDL_video.h>

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;
int screenWidth;
int screenHeight;

VertexArray* vertexArray;
VertexBuffer* vertexBuffer;
VertexBufferLayout* vertexBufferLayout;
IndexBuffer* indexBuffer;
Shader* shader;

float red, green, blue;
bool increaseRed, increaseGreen, increaseBlue;

void Renderer::ClearGLError()
{
	while (glGetError() != GL_NO_ERROR);
}

GLenum Renderer::CheckGLError()
{
	while (GLenum error = glGetError() != GL_NO_ERROR)
	{
		return error;
	}

	return GL_NO_ERROR;
}

static void SetSDLGlAttributes()
{
	bool result;

	result = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	ASSERT(result, true, SDL_GetError());
	result = SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	ASSERT(result, true, SDL_GetError());
	result = SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	ASSERT(result, true, SDL_GetError());

	result = SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	ASSERT(result, true, SDL_GetError());
	result = SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	ASSERT(result, true, SDL_GetError());
}

static void CreateSDLWindow()
{
	window = SDL_CreateWindow("Floxel", 64, 64, SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
	ASSERT(window == NULL, false, SDL_GetError());

	glContext = SDL_GL_CreateContext(window);
	ASSERT(glContext == NULL, false, SDL_GetError());

	int loadGladResult = gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
	ASSERT(loadGladResult, 1, "Failed loading glad");

	bool getWindowSizeResult = SDL_GetWindowSizeInPixels(window, &screenWidth, &screenHeight);
	ASSERT(getWindowSizeResult, true, SDL_GetError());
}

static void CreateGLData()
{
	/* Vertex positions (x, y) */
	float positions[] =
	{
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
		-0.5, 0.5f
	};

	/* Vertex indices */
	unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	/* 
	Creating vertex array buffer, 
	vertex buffer containing positions (2 floats for x, y * 4 for each corner of a rectangle), 
	vertex buffer layout buffer which we then push 2 floats which is the size of a position,
	we then add above vertex buffer layout to the vertex arraty,
	and lastly we create a index buffer to define how the triangles are to be draw.
	*/

	vertexArray = new VertexArray();
	vertexBuffer = new VertexBuffer(positions, 4 * 2 * sizeof(float));

	vertexBufferLayout = new VertexBufferLayout();
	vertexBufferLayout->Push(GL_FLOAT, 2, GL_FALSE);
	vertexArray->AddBuffer(*vertexBuffer, *vertexBufferLayout);

	indexBuffer = new IndexBuffer(indices, 6);

	red = 0.0f;
	green = 0.5f;
	blue = 1.0f;

	/* Getting shader assets, creating shader, binding and setting color uniform */
	Assets::TextAsset vertexShader = Resources::LoadTextAsset("shaders/BasicVertex.glsl");
	Assets::TextAsset fragmentShader = Resources::LoadTextAsset("shaders/BasicFragment.glsl");
	shader = new Shader(vertexShader, fragmentShader);
	shader->Bind();
	shader->SetUniformVector4("u_Color", Vector4(red, green, blue, 1.0f));

	/* Unbinds vertex array object, vertex buffer, index buffer and shader */
	vertexArray->Unbind();
	vertexBuffer->Unbind();
	indexBuffer->Unbind();
	shader->Unbind();
}

void Renderer::Initialize()
{
	SetSDLGlAttributes();
	CreateSDLWindow();
	CreateGLData();
}

void Renderer::Destroy()
{
	delete(vertexBufferLayout);
	delete(vertexArray);
	delete(vertexBuffer);
	delete(indexBuffer);
	delete(shader);
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
}

static void UpdateRectangleColor()
{
	constexpr float speed = 0.005f;

	red += speed * increaseRed - speed * !increaseRed;
	green += speed * increaseGreen - speed * !increaseGreen;
	blue += speed * increaseBlue - speed * !increaseBlue;

	if (red > 1.0f || red < 0.0f) increaseRed = !increaseRed;
	if (green > 1.0f || green < 0.0f) increaseGreen = !increaseGreen;
	if (blue > 1.0f || blue < 0.0f) increaseBlue = !increaseBlue;
}

void Renderer::RenderFrame()
{
	UpdateRectangleColor();

	/* Set viewport dimensions and clear back buffer */
	GL_CALL(glViewport(0, 0, screenWidth, screenHeight));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	/* Bind shader and update color uniform */
	shader->Bind();
	shader->SetUniformVector4("u_Color", Vector4(red, green, blue, 1.0f));

	/* Bind vertex array object and index buffer */
	vertexArray->Bind();
	indexBuffer->Bind();

	/* Draw to back buffer */
	GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	/* Swaps the front and back buffers, which renders the new frame */
	bool swapWindowResult = SDL_GL_SwapWindow(window);
	ASSERT(swapWindowResult, true, SDL_GetError());
}