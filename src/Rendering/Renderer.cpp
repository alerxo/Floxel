#include "Rendering/Renderer.h"

#include "Rendering/IndexBuffer.h"
#include "Rendering/Shader.h"
#include "Rendering/VertexArray.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/Texture.h"
#include "Resources.h"

#include <SDL3/SDL_video.h>

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;
int screenWidth;
int screenHeight;

VertexArray* rectangleVertexArray;
VertexBuffer* rectangleVertexBuffer;
VertexBufferLayout* rectangleVertexBufferLayout;
IndexBuffer* rectangleIndexBuffer;
Shader* rectangleShader;
Texture* texture;
Vector4 rectangleColor(0.0f, 0.5f, 1.0f, 1.0f);

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
		-0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, 1.0f, 0.0f,
		0.5f, 0.5f, 1.0f, 1.0f,
		-0.5, 0.5f, 0.0f, 1.0f,
	};

	/* Vertex indices */
	unsigned int indices[]
	{
		0, 1, 2,
		2, 3, 0
	};

	/* Enable blending */
	GL_CALL(glEnable(GL_BLEND));
	GL_CALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	/* 
	Creating vertex array buffer, 
	vertex buffer containing positions (2 floats for x, y * 4 for each corner of a rectangle), 
	vertex buffer layout buffer which we then push 2 floats which is the size of a position,
	we then add above vertex buffer layout to the vertex arraty,
	and lastly we create a index buffer to define how the triangles are to be draw.
	*/

	rectangleVertexArray = new VertexArray();
	rectangleVertexBuffer = new VertexBuffer(positions, 4 * 4 * sizeof(float));

	rectangleVertexBufferLayout = new VertexBufferLayout();
	rectangleVertexBufferLayout->Push(GL_FLOAT, 2, GL_FALSE);
	rectangleVertexBufferLayout->Push(GL_FLOAT, 2, GL_FALSE);
	rectangleVertexArray->AddBuffer(*rectangleVertexBuffer, *rectangleVertexBufferLayout);

	rectangleIndexBuffer = new IndexBuffer(indices, 6);

	/* Getting shader assets, creating shader, binding and setting color uniform */
	const Assets::TextAsset* vertexShader = Resources::LoadTextAsset("shaders/BasicVertex.glsl");
	const Assets::TextAsset* fragmentShader = Resources::LoadTextAsset("shaders/BasicFragment.glsl");
	rectangleShader = new Shader(vertexShader, fragmentShader);
	rectangleShader->Bind();
	rectangleShader->SetUniformVector4("u_Color", rectangleColor);

	/* Texture */
	const Assets::ImageAsset* image = Resources::LoadImageAsset("textures/toadhill_games_logo.bmp");
	texture = new Texture(image);
	texture->Bind(0);
	rectangleShader->SetUniformInt("u_Texture", 0);

	/* Unbinds vertex array object, vertex buffer, index buffer and shader (only in debug) */
	rectangleVertexArray->Unbind();
	rectangleVertexBuffer->Unbind();
	rectangleIndexBuffer->Unbind();
	rectangleShader->Unbind();
}

void Renderer::Initialize()
{
	SetSDLGlAttributes();
	CreateSDLWindow();
	CreateGLData();
}

void Renderer::Destroy()
{
	delete(rectangleVertexBufferLayout);
	delete(rectangleVertexArray);
	delete(rectangleVertexBuffer);
	delete(rectangleIndexBuffer);
	delete(rectangleShader);
	
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
}

static void UpdateRectangleColor()
{
	constexpr float speed = 0.005f;

	rectangleColor += Vector4(speed * increaseRed - speed * !increaseRed, speed * increaseGreen - speed * !increaseGreen, speed * increaseBlue - speed * !increaseBlue, 0.0f);

	if (rectangleColor.x > 1.0f || rectangleColor.x < 0.0f) increaseRed = !increaseRed;
	if (rectangleColor.y > 1.0f || rectangleColor.y < 0.0f) increaseGreen = !increaseGreen;
	if (rectangleColor.z > 1.0f || rectangleColor.z < 0.0f) increaseBlue = !increaseBlue;
}

static void Draw(const VertexArray* vertexArray, const IndexBuffer* indexBuffer, const Shader* shader)
{
	/* Bind shader, vertex array and index buffer */
	shader->Bind();
	vertexArray->Bind();
	indexBuffer->Bind();

	/* Draw to back buffer */
	GL_CALL(glDrawElements(GL_TRIANGLES, indexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr));
	
	/* Unbind shader, vertex array and index buffer (Only in debug) */
	shader->Unbind();
	vertexArray->Unbind();
	indexBuffer->Unbind();
}

void Renderer::RenderFrame()
{
	UpdateRectangleColor();

	/* Set viewport dimensions and clear back buffer */
	GL_CALL(glViewport(0, 0, screenWidth, screenHeight));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	rectangleShader->Bind();
	rectangleShader->SetUniformVector4("u_Color", rectangleColor);

	Draw(rectangleVertexArray, rectangleIndexBuffer, rectangleShader);

	/* Swaps the front and back buffers, which renders the new frame */
	bool swapWindowResult = SDL_GL_SwapWindow(window);
	ASSERT(swapWindowResult, true, SDL_GetError());
}