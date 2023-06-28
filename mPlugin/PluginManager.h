#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <vector>
#include "PluginBase.h"

class PluginManager
{
public:
	PluginManager();
	~PluginManager();

	void loadPlugins(const std::string& directory);
	void unloadPlugins();

	size_t getPluginCount() const;
	IPlugin* getPlugin(size_t index) const;
private:
	std::vector<HMODULE> pluginHandles_;
	std::vector<IPlugin*> plugins_;
};

