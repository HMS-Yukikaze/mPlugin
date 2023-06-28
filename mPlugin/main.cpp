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
    pluginManager.loadPlugins(pluginsPath.string());  // ������ڵ�Ŀ¼

    // ��ȡ�������
    size_t pluginCount = pluginManager.getPluginCount();
    std::cout << "Loaded " << pluginCount << " plugins." << std::endl;

    // ������ʹ�ò������
    for (size_t i = 0; i < pluginCount; ++i) {
        IPlugin* plugin = pluginManager.getPlugin(i);
        if (plugin) {
            plugin->initialize();
            plugin->execute();
        }
    }

    // ж�ز��
    pluginManager.unloadPlugins();
	return 0;
}

