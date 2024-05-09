#include "ShMemServer.hpp"


WinShmq& WinShmq::instance(const TCHAR* _name, std::size_t _size)
{
	static WinShmq  self(_name, _size);
	return self;
}

int WinShmq::CreateSharedMem()
{
	TCHAR buffer[256] = {};
	/* fail if already in use */
	hMapFile = OpenFileMapping(FILE_MAP_READ, false, mapName.c_str());
	if (hMapFile) {
		CloseHandle(hMapFile);
		return NULL;
	}

	hMapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,   // use paging file
		NULL,                   // default security
		PAGE_READWRITE,         // read/write access
		0,                      // maximum object size (high-order DWORD)
		totalSz,               // maximum object size (low-order DWORD)
		mapName.c_str());       // name of mapping object

	if (hMapFile == NULL)
	{
		// 处理错误
		wsprintf((LPWSTR)buffer, TEXT("Could not create file mapping object, [%d].\n"), GetLastError());
		//sprintf_s(mes, "Could not create file mapping object, [%d].\n", GetLastError());
		OutputDebugString(buffer);
		return -1;
	}

	pBuf = (Buffer*)MapViewOfFile(hMapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS,                              // read/write permission
		0,
		0,
		totalSz);

	if (pBuf == NULL)
	{
		// 处理错误
		wsprintf((LPWSTR)buffer, TEXT("Could not map view of file (%d).\n"), GetLastError());
		//sprintf_s(mes, "Could not map view of file (%d).\n", GetLastError());
		OutputDebugString(buffer);
		CloseHandle(hMapFile);
		return -1;
	}

	pBuf->head.cap = itemCount;
	pBuf->head.write_idx = pBuf->head.read_idx = 0;

	hWEvent = CreateEvent(NULL, TRUE, FALSE, WEVENT_NAME);
	if (hWEvent == NULL)
	{
		// 处理错误
		wsprintf((LPWSTR)buffer, TEXT("Could not Create Global Write Event, [%d].\n"), GetLastError());
		//sprintf_s(mes, "Could not Create Global Write Event, [%d].\n", GetLastError());
		OutputDebugString(buffer);
		destory();
		return -1;
	}
	return 0;
}


int WinShmq::Wait()
{
	while (1) {
		Message recv = {};
		OutputDebugString(TEXT("WaitForSingleObject\n"));
		DWORD dwWaitResult = WaitForSingleObject(hWEvent, INFINITE);

		switch (dwWaitResult) {
		case WAIT_OBJECT_0:
			EventHandle(recv);
			break;
		case WAIT_FAILED:
			printf("WaitForSingleObject failed (%d)\n", GetLastError());
			break;
		default:
			printf("Unexpected WaitForSingleObject return\n");
			break;
		}

		// 重置事件，以便下一次等待
		if (!ResetEvent(hWEvent)) {
			printf("ResetEvent failed (%d)\n", GetLastError());
			break;
		}
	}
	return 0;
}

int WinShmq::EventHandle(Message& msg)
{
	TCHAR content[MAX_PATH] = {};
	auto offset = pBuf->head.read_idx;
	memcpy(&msg, pBuf->buffer + offset, sizeof(Message));
	//更新读偏移量
	pBuf->head.read_idx = (pBuf->head.read_idx + 1) % (pBuf->head.cap);

	//auto sid = std::to_string(msg.id);
	//auto sID = utf8string2wstring(sid);

	wsprintf(content, TEXT("[EventHandle] Mes cam_id:%d type:%d\n"), msg.id, msg.type);
	OutputDebugString(content);
		
	return 0;
}


WinShmq::WinShmq(const TCHAR* _name, std::size_t _cap)
	:mapName{ _name },
	itemCount{ _cap },
	totalSz{ sizeof(Buffer) }
{
	hMapFile = nullptr;
	hWEvent = nullptr;
	pBuf = nullptr;	
	messageid = 0;
}

WinShmq::~WinShmq()
{
	destory();
}



void WinShmq::destory()
{
	if (pBuf) {
		UnmapViewOfFile(pBuf);
		pBuf = NULL;
	}

	if (hMapFile) {
		CloseHandle(hMapFile);
		hMapFile = NULL;
	}
	if (hWEvent) {
		CloseHandle(hWEvent);
	}

}
