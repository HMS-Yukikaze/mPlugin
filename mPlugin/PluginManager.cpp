#include "PluginManager.h"

PluginManager::PluginManager()
{
}

PluginManager::~PluginManager()
{
	unloadPlugins();
}

void PluginManager::loadPlugins(const std::string& directory)
{
    // 枚举目录下的所有 DLL 文件
    std::string searchPath = directory + "\\*.dll";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string pluginPath = directory + "\\" + findData.cFileName;

            // 加载插件 DLL
            HMODULE pluginHandle = LoadLibrary(pluginPath.c_str());
            if (pluginHandle) {
                // 获取插件创建函数指针
                CreatePlugin createPluginFunc = reinterpret_cast<CreatePlugin>(GetProcAddress(pluginHandle, "CreatePlugin"));
                if (createPluginFunc) {
                    // 创建插件实例
                    IPlugin* plugin = createPluginFunc();
                    if (plugin) {
                        std::cout << plugin->getName() <<"obj created..." << std::endl;
                        plugins_.push_back(plugin);
                        pluginHandles_.push_back(pluginHandle);
                    }
                }
            }
        } while (FindNextFile(hFind, &findData));

        FindClose(hFind);
    }
}

void PluginManager::unloadPlugins()
{
    for (IPlugin* plugin : plugins_) {
        delete plugin;
    }
    plugins_.clear();

    for (HMODULE pluginHandle : pluginHandles_) {
        FreeLibrary(pluginHandle);
    }
    pluginHandles_.clear();
}

size_t PluginManager::getPluginCount() const
{
    return plugins_.size();
}

IPlugin* PluginManager::getPlugin(size_t index) const
{
    if (index<plugins_.size()) {
        return plugins_[index];    
    }
    return nullptr;
}
