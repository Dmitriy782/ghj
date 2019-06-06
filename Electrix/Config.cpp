#include <fstream>
#include <ShlObj.h>

#include "archive.h"

#include "Config.h"

CFG::CFG(const char* name) noexcept
{
	PWSTR pathToDocuments;

	if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pathToDocuments)))
	{
		path = pathToDocuments;
		path /= name;
		CoTaskMemFree(pathToDocuments);
	}

	if (!std::filesystem::is_directory(path)) 
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::transform(std::filesystem::directory_iterator{ path }, std::filesystem::directory_iterator{ }, std::back_inserter(cfgs), [](const auto & entry) { return entry.path().filename().string(); });
}

void CFG::Add(const char* name) noexcept
{
	if (*name && std::find(std::cbegin(cfgs), std::cend(cfgs), name) == std::cend(cfgs))
		cfgs.emplace_back(name);
}

void CFG::Remove(size_t id) noexcept
{
	std::filesystem::remove(path / cfgs[id]);
	cfgs.erase(cfgs.cbegin() + id);
}

void CFG::Save(size_t id) const noexcept
{
	if (!std::filesystem::is_directory(path)) 
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ofstream ofile{ path / cfgs[id] };

	if (!ofile.good())
		return;

	Archive<std::ofstream>{ ofile } << aimbot << extras;
	
	ofile.close();
}

void CFG::Load(size_t id) noexcept
{
	if (!std::filesystem::is_directory(path)) 
	{
		std::filesystem::remove(path);
		std::filesystem::create_directory(path);
	}

	std::ifstream ifile{ path / cfgs[id] };

	if (!ifile.good())
		return;

	Archive<std::ifstream>{ ifile } >> aimbot >> extras;

	ifile.close();
}

void CFG::SetName(size_t item, const char* newName) noexcept
{
	std::filesystem::rename(path / cfgs[item], path / newName);
	cfgs[item] = newName;
}