#pragma once
#include <cstdint>
#include <string>

class PluginConfig {
public:
	PluginConfig();
	int32_t ConfigValue;

	bool IsLoaded() const;
	void ReloadFromConfig();
private:
	const wchar_t* ConfigPath;
	bool Loaded;
	static std::string GetDocumentsDirectory();
	void Initialize();
};

extern PluginConfig g_PluginConfig;