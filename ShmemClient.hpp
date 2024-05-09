#pragma once
#include <windows.h>
#include <string>

#include "utils.h"

class ShmClient
{
private:
    HANDLE hMapFile;
    HANDLE hWEvent;   
    int itemCount;   
    std::size_t totalSz;   
    Buffer* pBuf;     /* shared memory mapping */
    std::wstring mapName;
public:
    static ShmClient& instance();
    
    int openMem();

    bool isFull();
    
    int sendMsg(Message& src);
private:
    ShmClient(const TCHAR*_name = MAPPING_NAME, int _cap = DEFAULT_CAP);

    ~ShmClient();

    void destory();
};
