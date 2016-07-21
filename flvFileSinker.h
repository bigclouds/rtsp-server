#pragma once
#include "realtimeSinker.h"
#include "flvWriter.h"

//�ļ��ۣ�ͨ���������
//��һ��д��ؼ�֡�󣬷����浵֪ͨ
//������ļ�������ʱ�����浵����֪ͨ����֪ͨԴ��û����
class flvFileSinker :
	public streamSinker
{
public:
	flvFileSinker(taskJsonStruct &stTask,int jsonSocket);
	virtual ~flvFileSinker();
	virtual	taskJsonStruct& getTaskJson();
	virtual void addDataPacket(DataPacket &dataPacket);
	virtual void addTimedDataPacket(timedPacket &dataPacket);
	virtual void sourceRemoved();//��֪ͨԴ�Ѿ�����
	virtual int handTask(taskScheduleObj *task_obj);
	bool inited();
private:
	void initFlvSinker();
	void notifyStartSave();
	void notifyStopSave();
	flvWriter *m_flvWriter;
	taskJsonStruct	m_taskJson;
	std::string m_fullFileName;
	bool m_inited;
	int m_jsonSocket;
	unsigned int m_firstPktTime;
};

