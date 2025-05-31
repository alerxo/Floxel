#include "Rendering/Shader.h"

#include "Rendering/Renderer.h"

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
    Should technically be detached but this removed debugging features
    glDetachShader(program, vs);
    glDetachShader(program, fs);
    */

    return program;
}

Shader::Shader(const Assets::TextAsset* vertexShader, const Assets::TextAsset* fragmentShader) : vertexShaderAsset(vertexShader), fragmentShaderAsset(fragmentShader), rendererID(0)
{
    rendererID = CreateShader(vertexShader->text, fragmentShader->text);
}

Shader::~Shader()
{
    GL_CALL(glDeleteProgram(rendererID));
}

void Shader::Bind() const
{
    GL_CALL(glUseProgram(rendererID));
}

void Shader::Unbind() const
{
#ifdef DEBUG
    GL_CALL(glUseProgram(0));
#endif // DEBUG
}

int Shader::GetUniformLocation(std::string name)
{
    int location;

    if (uniformLocationCache.contains(name))
    {
        location = uniformLocationCache[name];
    }

    else
    {
        GL_CALL(location = glGetUniformLocation(rendererID, name.c_str()));
        uniformLocationCache[name] = location;
    }

    if (location == -1)
    {
        LOG_WARNING(std::format("Shader uniform location ({}) not found, could be unused!", name));
    }
    
    return location;
}

void Shader::SetUniformVector4(std::string name, Vector4 vector4)
{
    GL_CALL(glUniform4f(GetUniformLocation(name), vector4.w, vector4.x, vector4.y, vector4.z))
}
void Shader::SetUniformInt(std::string name, int value)
{
    GL_CALL(glUniform1i(GetUniformLocation(name), value));
}