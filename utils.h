#pragma once
#include <Windows.h>
#include <memory.h>
#include <tchar.h>
#include <stdio.h>

constexpr int DEFAULT_CAP = 100;
constexpr const TCHAR MAPPING_NAME[]=TEXT("Global\\MY_MAPPING");
constexpr const TCHAR WEVENT_NAME[]=TEXT("Global\\MY_MAPPINGEVENT");

enum queue_state {
	SHARED_QUEUE_STATE_INVALID,
	SHARED_QUEUE_STATE_STARTING,
	SHARED_QUEUE_STATE_READY,
	SHARED_QUEUE_STATE_STOPPING,
};


struct Message
{    
    int id;                    //id
    int type;                  //类型
    char uuid[64];             //uuid
    char data[260];            //消息 
};

struct Header {
    int write_idx;
    int read_idx;
    int cap;
};


struct Buffer {
    Header head;    
    Message buffer[DEFAULT_CAP];
};

