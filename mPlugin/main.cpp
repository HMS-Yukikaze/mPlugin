#include <Windows.h>
#include <iostream>
#include <filesystem>
#include "PluginManager.h"


using namespace std;

int main(int argc,char*argv[]) {
	//example
    auto curPath =std::filesystem::current_path();
    auto pluginsPath = curPath.append("plugins");
    PluginManager pluginManager;
    pluginManager.loadPlugins(pluginsPath.string());  // 插件所在的目录

    // 获取插件数量
    size_t pluginCount = pluginManager.getPluginCount();
    std::cout << "Loaded " << pluginCount << " plugins." << std::endl;

    // 遍历并使用插件功能
    for (size_t i = 0; i < pluginCount; ++i) {
        IPlugin* plugin = pluginManager.getPlugin(i);
        if (plugin) {
            plugin->initialize();
            plugin->execute();
        }
    }

    // 卸载插件
    pluginManager.unloadPlugins();
	return 0;
}

