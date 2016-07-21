#pragma once

#include "task_executor.h"
#include "taskJsonStructs.h"
#include "task_schedule_obj.h"
#include "RTP.h"

class streamSinker :public taskExecutor
{
public:
	//������ȥ����
	streamSinker(taskJsonStruct &stTask);
	virtual	taskJsonStruct& getTaskJson() = 0;
	virtual void addDataPacket(DataPacket &dataPacket);
	virtual void addTimedDataPacket(timedPacket &dataPacket);
	virtual void sourceRemoved()=0;//��֪ͨԴ�Ѿ�����
private:
	streamSinker(const streamSinker&) = delete;
	streamSinker& operator=(const streamSinker&) = delete;
};