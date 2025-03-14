#include "PluginConfig.h"
#include "SimpleIni.h"
#include "xorstr.hpp"
#include <filesystem>
#ifdef _DEBUG
#include <iostream>
#endif // _DEBUG

PluginConfig g_PluginConfig;

PluginConfig::PluginConfig()
{
	Initialize();
}

std::string PluginConfig::GetDocumentsDirectory() {
	if (const char* userProfile = std::getenv("USERPROFILE"); userProfile != nullptr) {
		return std::string(userProfile) + "\\Documents";
	}
	MessageBox(nullptr, L"Could not find Documents directory.", L"AnimFilter Fatal Error", MB_OK | MB_ICONERROR);
	return "";
}

void PluginConfig::Initialize() {
	WCHAR fullpath[MAX_PATH];
	GetModuleFileNameW(nullptr, fullpath, MAX_PATH);
	std::filesystem::path base(fullpath);
	std::wstring inipath(base.parent_path());
	inipath += xorstr_(L"/plugins/templateconfig.ini");
	ConfigPath = inipath.c_str();
#ifdef _DEBUG
	std::wcout << ConfigPath << std::endl;
#endif // _DEBUG
}

void PluginConfig::ReloadFromConfig()
{
	CSimpleIniA configIni;
	configIni.SetUnicode();
	configIni.LoadFile(ConfigPath);
	if (configIni.GetValue("SECTION", "Key"))
	{
		ConfigValue = std::stoi(configIni.GetValue("SECTION", "Key"));
	}
	Loaded = true;
}

bool PluginConfig::IsLoaded() const
{
	return Loaded;
}