#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <format>
#include <sstream>
#include <unordered_map>

#include "stb_image.h"

constexpr const char *targetResourceDirectory = "resources/";
constexpr const char *templateFile = "dependencies/Assets/AssetPacker/resources/AssetsTemplate.cpp";
constexpr const char *sourceDirectory = "build/_intermediate/AssetPacker/src/";
constexpr const char *sourcePath = "build/_intermediate/AssetPacker/src/Assets.cpp";

enum class AssetType
{
	Binary = 0,
	Text,
	BMP,
	PNG,
};

std::unordered_map<std::string, const AssetType> assetTypeLists = {
	{".glsl", AssetType::Text},
	{".png", AssetType::PNG}};

/* Directories */
void CreateDirectories()
{
	std::filesystem::create_directories(sourceDirectory);
}

const std::vector<std::string> GetDirectories()
{
	std::vector<std::string> directories;

	for (auto &entry : std::filesystem::directory_iterator(targetResourceDirectory))
	{
		if (entry.is_directory())
		{
			directories.push_back(entry.path().string());
		}
	}

	return directories;
}

const std::vector<std::string> GetFilePaths()
{
	std::vector<std::string> filePaths;
	const std::vector<std::string> directories = GetDirectories();

	for (auto directory = directories.begin(); directory != directories.end(); ++directory)
	{
		for (auto &entry : std::filesystem::directory_iterator(*directory))
		{
			filePaths.push_back(entry.path().string());
		}
	}

	return filePaths;
}

/* String util */
void ReplaceSubString(std::string &target, const std::string from, const std::string to)
{
	const size_t start = target.find(from);

	if (start == std::string::npos)
	{
		std::cerr << "\nfailed replacing string: (target) " << target << ", (from) " << from << std::endl;

		return;
	}

	target.replace(start, from.length(), to);
}

void EraseSubString(std::string &target, const std::string from)
{
	const size_t start = target.find(from);

	if (start == std::string::npos)
	{
		std::cerr << "\nfailed replacing string: (target) " << target << ", (from) " << from << std::endl;

		return;
	}

	target.erase(start, from.length());
}

void FormatPath(std::string &target)
{
	for (int i = 0; i < target.size(); i++)
	{
		if (target[i] == '\\')
		{
			target[i] = '/';
		}
	}
}

const std::string GetMethodName(const std::string filePath)
{
	std::string result = filePath;

	EraseSubString(result, targetResourceDirectory);
	FormatPath(result);

	return result;
}

/* Definition */
void LoadTemplate(std::string *target)
{
	/* Reading each line of template and putting it in the target file*/
	std::ifstream in(templateFile);
	std::string line;

	while (std::getline(in, line))
	{
		*target += (line + "\n");
	}
}

const AssetType GetType(std::string filePath)
{
	std::size_t found = filePath.find(".");
	filePath.erase(0, found);

	if (!assetTypeLists.contains(filePath))
	{
		std::cout << "Error: Unsuported file type: " << filePath << std::endl;

		return AssetType::Binary;
	}

	return assetTypeLists[filePath];
}

const bool CheckType(const std::string &name, const AssetType type)
{
	if (GetType(name) != type)
	{
		return false;
	}

	std::cout << "[AssetPacker] Packing " << name << "\n";

	return true;
}

void LoadTextFiles(std::string &target, const std::vector<std::string> filePaths)
{
	std::stringstream result;

	for (auto filePath = filePaths.begin(); filePath != filePaths.end(); ++filePath)
	{
		const std::string name = GetMethodName(*filePath);

		if (!CheckType(name, AssetType::Text))
		{
			continue;
		}

		/* The map key */
		result << "    {\"" + name + "\", ";

		/* TextAsset::name */
		result << "{\"" + name + "\",";

		/* TextAsset::text */
		result << " \"";

		/* Reading each line of file and adding it to result */
		std::ifstream stream(*filePath);
		std::string line;

		while (std::getline(stream, line))
		{
			result << line + "\\n";
		}

		result << "\"";

		/* Brackets */
		result << "}},\n";
	}

	ReplaceSubString(target, "//Text", result.str());
}

void LoadImageFiles(std::string &target, const std::vector<std::string> filePaths)
{
	std::stringstream result;

	for (auto filePath = filePaths.begin(); filePath != filePaths.end(); ++filePath)
	{
		const std::string name = GetMethodName(*filePath);

		if (!CheckType(name, AssetType::PNG))
		{
			continue;
		}

		int width = 0;
		int height = 0;
		int bpp = 0;

		stbi_set_flip_vertically_on_load(1);
		unsigned char *buffer = stbi_load(filePath->c_str(), &width, &height, &bpp, 4);

		/* The map key */
		result << "    {\"" << name << "\", ";

		/* ImageAsset::name */
		result << "{\"" << name << "\",";

		/* ImageAsset::width, ImageAsset::height */
		result << " " << width << ", " << height << ", ";

		/* ImageAsset::buffer */
		result << " {";

		for (int i = 0; i < height * width * 4; i++)
		{
			result << (unsigned int)buffer[i] << ", ";
		}

		result << "}";

		/* Brackets */
		result << "}},\n";
	}

	ReplaceSubString(target, "//Image", result.str());
}

/* Create definition file */
void CreateDefinition(const std::string &source)
{
	std::ofstream sourceFile;
	sourceFile.open(sourcePath, std::fstream::out);

	if (sourceFile.is_open())
	{
		sourceFile.clear();
	}

	else
	{
		std::cerr << "\nfailed opening target file: " << templateFile << std::endl;
	}

	sourceFile << source;
	sourceFile.close();
}

void CreateSourceFile()
{
	std::string source;
	LoadTemplate(&source);

	const std::vector<std::string> filePaths = GetFilePaths();
	LoadTextFiles(source, filePaths);
	LoadImageFiles(source, filePaths);

	CreateDefinition(source);
}

int main()
{
	CreateDirectories();
	CreateSourceFile();

	return 0;
}