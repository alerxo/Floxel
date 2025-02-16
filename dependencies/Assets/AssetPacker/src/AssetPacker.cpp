#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <filesystem>
#include <unordered_map>

constexpr const char *targetResourceDirectory = "resources/";
constexpr const char *templateFile = "dependencies/Assets/AssetPacker/resources/AssetsTemplate.cpp";
constexpr const char *sourceDirectory = "build/_intermediate/AssetPacker/src/";
constexpr const char *sourcePath = "build/_intermediate/AssetPacker/src/Assets.cpp";

enum class AssetType
{
	Binary = 0,
	Text,
	Image,
};

std::unordered_map<std::string, const AssetType> assetTypeLists =
{
	{".glsl", AssetType::Text},
	{".png", AssetType::Image}
};

/* Directories*/
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

/* String util*/
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

AssetType GetType(std::string filePath)
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

void LoadBinaryFiles(std::ofstream *target, const std::vector<std::string> filePaths)
{
	std::cout << "\n";
	for (auto filePath = filePaths.begin(); filePath != filePaths.end(); ++filePath)
	{
		const std::string name = GetMethodName(*filePath);
		std::cout << "Packing " << name << "\n";
		*target << "    {\"" << name << "\", {";

		std::ifstream inputFile(*filePath, std::ios_base::binary);

		inputFile.seekg(0, std::ios_base::end);
		auto length = inputFile.tellg();
		inputFile.seekg(0, std::ios_base::beg);

		std::vector<unsigned char> buffer(length);
		inputFile.read(reinterpret_cast<char *>(buffer.data()), length);
		inputFile.close();

		for (size_t i = 0; i < buffer.size() - 1; i++)
		{
			*target << "0x" << std::hex << (int)buffer.at(i);
			*target << ", ";
		}

		*target << "0x" << std::hex << (int)buffer.at(buffer.size() - 1) << "}},\n";
	}

	std::cout << "\n";
}

void LoadTextFiles(std::string& target, const std::vector<std::string> filePaths)
{
	std::cout << "\n";
	std::string textFiles;

	for (auto filePath = filePaths.begin(); filePath != filePaths.end(); ++filePath)
	{
		const std::string name = GetMethodName(*filePath);

		if (GetType(name) != AssetType::Text)
		{
			continue;
		}

		std::cout << "Packing " << name << "\n";

		/* The start of the line*/
		textFiles += "    {\"" + name + "\", {\"" + name + "\", \"";

		/* Reading each line of file and putting it in the target file*/
		std::ifstream in(*filePath);
		std::string line;

		while (std::getline(in, line))
		{
			textFiles += line + "\\n";
		}

		/* The end of the line */
		textFiles += filePath + 1 != filePaths.end() ? "\"}},\n" : "\"}}";
	}

	ReplaceSubString(target, "//Text", textFiles);
	std::cout << "\n";
}

/* Create definition file */
void CreateDefinition(const std::string& source)
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

	CreateDefinition(source);
}

int main()
{
	CreateDirectories();
	CreateSourceFile();

	return 0;
}