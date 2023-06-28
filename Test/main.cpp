#include <iostream>
#include <windows.h> // Windows平台下的头文件
#include "TestInterface.h"

typedef IPlugin* (*CreatePlugin)(); // 定义函数指针类型

int main() {
    HINSTANCE hDLL = LoadLibrary("Interface_D.dll"); // 加载DLL
    if (hDLL == nullptr) {
        std::cout << "Failed to load DLL." << std::endl;
        return 1;
    }

    CreatePlugin createPluginFunc = reinterpret_cast<CreatePlugin>(GetProcAddress(hDLL, "CreatePlugin")); // 获取函数指针
    if (createPluginFunc == nullptr) {
        std::cout << "Failed to get function pointer." << std::endl;
        return 1;
    }

    IPlugin* plugin = createPluginFunc(); // 调用函数创建插件对象

    // 调用其他插件函数
    std::cout << "Plugin Name: " << plugin->getName() << std::endl;
    plugin->initialize();
    plugin->execute();

    delete plugin; // 删除插件对象

    FreeLibrary(hDLL); // 释放DLL

    return 0;
}
