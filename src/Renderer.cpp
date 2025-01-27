#include "Renderer.h"

#include "Debug.h"
#include "Resources.h"
#include <SDL3/SDL_video.h>
#include <format>

#define GL_CALL(x) ClearGLError(); x; { GLenum glCallResult = CheckGLError(); ASSERT(glCallResult, GL_NO_ERROR, std::format("OpenGL Call error for ({})", #x)); }

SDL_Window *window = nullptr;
SDL_GLContext glContext = nullptr;
int screenWidth;
int screenHeight;

unsigned int shader;
int colorLocation;

VertexArray* vertexArray;
VertexBuffer* vertexBuffer;
VertexBufferLayout* vertexBufferLayout;
IndexBuffer* indexBuffer;

float red, green, blue;
bool increaseRed, increaseGreen, increaseBlue;

static void ClearGLError()
{
	while (glGetError() != GL_NO_ERROR);
}

static GLenum CheckGLError()
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

static unsigned int CompileShader(unsigned int type, const std::string shaderSource)
{
	/* Creating and compiling shader from char array */
	GL_CALL(unsigned int id = glCreateShader(type));
	const char *source = shaderSource.c_str();
	GL_CALL(glShaderSource(id, 1, &source, nullptr));
	GL_CALL(glCompileShader(id));

	/* Error handling */
	int result;
	GL_CALL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GL_CALL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char *message = (char *)malloc(sizeof(char) * length);
		GL_CALL(glGetShaderInfoLog(id, length, &length, message));

		std::string typeMessage;

		switch (type)
		{
		case GL_VERTEX_SHADER:
			typeMessage = "vertex";
			break;

		case GL_FRAGMENT_SHADER:
			typeMessage = "fragment";
			break;
		default:
			break;
		}

		LOG_ERROR(std::format("OpenGL {} shader compilation error ({})", typeMessage, message));

		free(message);
		GL_CALL(glDeleteShader(id));

		return 0;
	}

	return id;
}

static unsigned int CreateShader(const std::string &vertexShaderSource, const std::string &fragmentShaderSource)
{
	/* Creating gl program and compiling shader */
	GL_CALL(unsigned int program = glCreateProgram());
	unsigned int vertexShader = CompileShader(GL_VERTEX_SHADER, vertexShaderSource);
	unsigned int fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

	/* Linking compiled shaders to gl program */
	GL_CALL(glAttachShader(program, vertexShader));
	GL_CALL(glAttachShader(program, fragmentShader));
	GL_CALL(glLinkProgram(program));
	GL_CALL(glValidateProgram(program));

	/* Deleting shaders as they have been linked */
	GL_CALL(glDeleteShader(vertexShader));
	GL_CALL(glDeleteShader(fragmentShader));

	/*
	* Should technically be detached but this removed debugging features
	glDetachShader(program, vs);
	glDetachShader(program, fs);
	*/

	return program;
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

	/* Binding Shader */
	Assets::TextAsset vertextShader = Resources::LoadTextAsset("shaders/BasicVertex.glsl");
	Assets::TextAsset fragmentShader = Resources::LoadTextAsset("shaders/BasicFragment.glsl");

	shader = CreateShader(vertextShader.text, fragmentShader.text);
	GL_CALL(glUseProgram(shader));

	/* Get color uniform location */
	GL_CALL(colorLocation = glGetUniformLocation(shader, "u_Color"));

	if (colorLocation == -1)
	{
		LOG_WARNING("Shader color location not found, could be unused!");
	}

	/* Unbinds vao, shader, vertex buffer, index buffer */
	GL_CALL(glUseProgram(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	red = 0.0f;
	green = 0.5f;
	blue = 1.0f;
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
	GL_CALL(glDeleteProgram(shader));
	SDL_GL_DestroyContext(glContext);
	SDL_DestroyWindow(window);
}

void Renderer::RenderFrame()
{
	/* Color of the rectangle */
	constexpr float speed = 0.005f;

	red += speed * increaseRed - speed * !increaseRed;
	green += speed * increaseGreen - speed * !increaseGreen;
	blue += speed * increaseBlue - speed * !increaseBlue;

	if (red > 1.0f || red < 0.0f) increaseRed = !increaseRed;
	if (green > 1.0f || green < 0.0f) increaseGreen = !increaseGreen;
	if (blue > 1.0f || blue < 0.0f) increaseBlue = !increaseBlue;

	/* Set viewport dimensions and clear back buffer */
	GL_CALL(glViewport(0, 0, screenWidth, screenHeight));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT));

	/* Bind shadeer and set color uniform */
	GL_CALL(glUseProgram(shader));
	GL_CALL(glUniform4f(colorLocation, red, green, blue, 1.0f));

	/* Bind vertex array object and index buffer */
	vertexArray->Bind();
	indexBuffer->Bind();

	/* Draw to back buffer */
	GL_CALL(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

	/* Swaps the front and back buffers, which renders the new frame */
	bool swapWindowResult = SDL_GL_SwapWindow(window);
	ASSERT(swapWindowResult, true, SDL_GetError());
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	/* Creating buffer and filling it with vertex position data */
	GL_CALL(glGenBuffers(1, &renderID));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, renderID));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &renderID));
}

void VertexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, renderID));
}

void VertexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int count) : count(count)
{
	ASSERT(sizeof(unsigned int), sizeof(GLuint), "Mismatch of type size");

	GL_CALL(glGenBuffers(1, &renderID));
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
	GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * count, data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GL_CALL(glDeleteBuffers(1, &renderID));
}

void IndexBuffer::Bind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderID));
}

void IndexBuffer::Unbind() const
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned int VertexBufferElement::GetSizeOfType(unsigned int type)
{
	switch (type)
	{
	case GL_FLOAT:
		return 4;

	case GL_UNSIGNED_INT:
		return 4;

	case GL_UNSIGNED_BYTE:
		return 1;

	default:
		LOG_ERROR("Defaulted");
		return 0;
	}
}

void VertexBufferLayout::Push(unsigned int type, unsigned int count, unsigned char isNormalized)
{
	elements.push_back({type, count, isNormalized});
	stride += VertexBufferElement::GetSizeOfType(type) * count;
}

VertexArray::VertexArray()
{
	GL_CALL(glGenVertexArrays(1, &rendererID));
	GL_CALL(glBindVertexArray(rendererID));
}

VertexArray::~VertexArray()
{
	GL_CALL(glDeleteVertexArrays(1, &rendererID));
}

void VertexArray::AddBuffer(const VertexBuffer &buffer, const VertexBufferLayout &layout)
{
	Bind();
	buffer.Bind();

	/* 
	Enabling and specifiyng the buffer layout of buffer using all elements
	Keeps track of offset with element size
	Attributes will be bound to vertex array object
	*/
	const std::vector<VertexBufferElement>& elements = layout.GetElements();
	intptr_t offset = 0;

	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const VertexBufferElement element = elements[i];
		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(0, element.count, element.type, element.isNormalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GL_CALL(glBindVertexArray(rendererID));
}

void VertexArray::Unbind() const
{
	GL_CALL(glBindVertexArray(0));
}