#include "ShmemClient.hpp"

ShmClient& ShmClient::instance()
{
	static ShmClient self;
	return self;
}

ShmClient::ShmClient(const TCHAR* _name, int _cap)
	: mapName{ _name },
	itemCount{ _cap },	
	totalSz{ sizeof(Buffer)}
{
	hMapFile = nullptr;
	pBuf = nullptr;
	hWEvent = nullptr;	
}

ShmClient::~ShmClient()
{
	destory();
}

void ShmClient::destory()
{
	if (pBuf)
	{
		UnmapViewOfFile(pBuf);
		pBuf = NULL;
	}

	if (hMapFile)
	{
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	if (hWEvent)
	{
		CloseHandle(hWEvent);
		hWEvent = NULL;
	}
}

int ShmClient::openMem()
{
	int re = 0;
	TCHAR buffer[256] = {};

	hMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS, // read/write access
		FALSE,               // do not inherit the name
		mapName.c_str());    // name of mapping object

	if (hMapFile == NULL)
	{
		// 处理错误		
		wsprintf(buffer, TEXT("Could not OpenFileMapping, [%d].\n"), GetLastError());
		OutputDebugString(buffer);
		return -1;
	}

	if (totalSz == 0) {
		// 处理错误
		wsprintf(buffer, TEXT("FileMapping Could not be zero\n"));
		OutputDebugString(buffer);
		return -1;
	}

	pBuf = (Buffer*)MapViewOfFile(hMapFile,            // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		totalSz);

	if (pBuf == NULL)
	{
		// 处理错误
		wsprintf(buffer, TEXT("Could not MapViewOfFile, [%d].\n"), GetLastError());
		OutputDebugString(buffer);

		CloseHandle(hMapFile);

		return 1;
	}

	hWEvent = OpenEvent(EVENT_ALL_ACCESS, false, WEVENT_NAME);
	if (hWEvent == NULL) {
		// 处理错误
		wsprintf(buffer, TEXT("Could not OpenEvent, [%d].\n"), GetLastError());
		OutputDebugString(buffer);
		return 1;
	}


	return 0;
}

bool ShmClient::isFull()
{
	return (pBuf->head.write_idx + 1) % itemCount == (pBuf->head.read_idx % itemCount) ? true : false;
}

int ShmClient::sendMsg(Message& src)
{
	if (isFull()) {
		return -1;
	}
	
	// 根据偏移量写入消息
	CopyMemory((void*)(pBuf->buffer + pBuf->head.write_idx), &src, (sizeof(Message)));

	// 更新写偏移量
	pBuf->head.write_idx = (pBuf->head.write_idx + 1) % itemCount;

	if (!SetEvent(hWEvent)) {
		printf("SetEvent failed (%d)\n", GetLastError());
		return -1;
	}
	return 0;
}
