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
    // ö��Ŀ¼�µ����� DLL �ļ�
    std::string searchPath = directory + "\\*.dll";
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string pluginPath = directory + "\\" + findData.cFileName;

            // ���ز�� DLL
            HMODULE pluginHandle = LoadLibrary(pluginPath.c_str());
            if (pluginHandle) {
                // ��ȡ�����������ָ��
                CreatePlugin createPluginFunc = reinterpret_cast<CreatePlugin>(GetProcAddress(pluginHandle, "CreatePlugin"));
                if (createPluginFunc) {
                    // �������ʵ��
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
