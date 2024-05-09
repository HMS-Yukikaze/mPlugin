#pragma once
#include <Windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <string>
#include "utils.h"

#include "StringHelper.hpp"
#include "SafeQueue.hpp"



class WinShmq
{
private:
	int messageid;
	HANDLE hMapFile;
	HANDLE hWEvent;    //write event      
	Buffer* pBuf;
	std::size_t itemCount;
	std::size_t totalSz;
	std::wstring mapName;	
public:
	static WinShmq& instance(const TCHAR* _name, std::size_t _size);

	int CreateSharedMem();

	int Wait();

	int EventHandle(Message& msg);
	
private:
	WinShmq(const TCHAR* _name = MAPPING_NAME, std::size_t _cap = DEFAULT_CAP);

	~WinShmq();
	// 防止复制和赋值操作
	WinShmq(const WinShmq&) = delete;
	WinShmq& operator=(const WinShmq&) = delete;

	void destory();
};



