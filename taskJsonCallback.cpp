#include "RTSP_server.h"
#include "taskSchedule.h"
#include "tcpCacher.h"
#include "task_json.h"

#include <thread>

tcpCacherManager &taskJsonCacher()
{
	static tcpCacherManager cacherManager;
	return cacherManager;
}

void taskJsonCallback(socket_event_t * data)
{
	//�׽��ֹرգ�
	if (data->sock_closed)
	{
		taskJsonCloseClient(data->clientId);
		return;
	}
	taskJsonCacher().appendBuffer(data->clientId, (char*)data->dataPtr, data->dataLen);
	//ת�����������
	std::list<DataPacket> dataList;
	taskJsonCacher().getValidBuffer(data->clientId, dataList, parseTaskJsonData);
	for (auto it:dataList)
	{
		std::string strJson=it.data + 4;
		//��Ϊ���ݹ���������û�н������������ֶ�����
		strJson.resize(it.size - 4);
		taskScheduleObj *taskObj = new task_json(strJson, data->clientId);
		
		taskSchedule::getInstance().addTaskObj(taskObj);
	}
}

void taskJsonCloseClient(socket_t sockId)
{
	taskJsonCacher().removeCacher(sockId);
	taskJsonClosed *taskClosed = new taskJsonClosed(sockId);
	//taskSchedule::getInstance().addTaskObj(taskClosed);
	taskSchedule::getInstance().executeTaskObj(taskClosed);
	delete taskClosed;
}