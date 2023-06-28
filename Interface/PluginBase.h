#pragma once
#include <string>

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif

class DLL_EXPORT IPlugin {
public:
    virtual ~IPlugin() {}
    virtual std::string getName() = 0;
    virtual void initialize() = 0;
    virtual void execute() = 0;
    
};

// 导出插件的创建函数
extern "C" DLL_EXPORT IPlugin * CreatePlugin();