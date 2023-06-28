#pragma once
#include <typeinfo>
#include <iostream>
#include "PluginBase.h"

class __declspec(dllexport)TestInterface :public IPlugin
{
public:
	std::string getName()override;
	void initialize()override;
	void execute()override;
};


