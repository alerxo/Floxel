#pragma once

#include "Assets.h"
#include "Maths/Vector4.h"
#include <unordered_map>

class Shader
{
private:
	const Assets::TextAsset* vertexShaderAsset;
	const Assets::TextAsset* fragmentShaderAsset;
	unsigned int rendererID;
	std::unordered_map<std::string, int> uniformLocationCache;

public:
	Shader(const Assets::TextAsset*, const Assets::TextAsset*);
	~Shader();

	void Bind() const;
	void Unbind() const;
	int GetUniformLocation(std::string);
	void SetUniformVector4(std::string, Vector4);
	void SetUniformInt(std::string, int);
};