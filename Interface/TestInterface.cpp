#include "TestInterface.h"



std::string TestInterface::getName()
{	
	return std::string("TestInterface");
}

void TestInterface::initialize()
{
	std::cout<< "initialize func entry..." << std::endl;
	//todo:��ӳ�ʼ������
}

void TestInterface::execute()
{
	std::cout << "execute func..." << std::endl;
	//todo:��ӳ�ʼ������
}

// ��������Ĵ�������
extern "C" DLL_EXPORT IPlugin * CreatePlugin() {
	return new TestInterface();
}