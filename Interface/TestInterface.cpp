#include "TestInterface.h"



std::string TestInterface::getName()
{	
	return std::string("TestInterface");
}

void TestInterface::initialize()
{
	std::cout<< "initialize func entry..." << std::endl;
	//todo:添加初始化代码
}

void TestInterface::execute()
{
	std::cout << "execute func..." << std::endl;
	//todo:添加初始化代码
}

// 导出插件的创建函数
extern "C" DLL_EXPORT IPlugin * CreatePlugin() {
	return new TestInterface();
}