#ifndef _TASK_SCHEDULE_H_
#define _TASK_SCHEDULE_H_

#include "task_schedule_obj.h"
#include "task_executor.h"

#include <list>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <map>

class taskSchedule
{
public:
	static taskSchedule &getInstance();
	taskSchedule();
	virtual ~taskSchedule();
	//�������������У��첽
	void addTaskObj(taskScheduleObj *taskObj);
	//ִ������ͬ�����ؽ��
	void executeTaskObj(taskScheduleObj *taskObjInOut);
protected:
private:
	taskSchedule(taskSchedule&)=delete;
	void taskScheduleInit();
	void taskScheduleShutdown();
	void addTaskObjPri(taskScheduleObj *task_obj);
	//��Ϊֱ�������Json�������
	//ֱ������������ļ��Ͳ�ֱ�������ͻ��ˣ���ʱ�̣�ֱ�ӵ��߳�
	//Json����������� ���ļ��ȣ����ܺ�ʱ�ܳ������첽
	///ֱ������
	//�ַ�����;
	static void taskLivingDistributionThread(void *lparam);
	//����ɾ������;
	void taskLivingHandler(taskScheduleObj *task_obj);
	//������;
	//��ʼһ���ļ�ֱ��;
	void startLivingStreamFile(taskScheduleObj *task_obj);
	//����һ���ļ�ֱ��;
	void stopLivingStreamFile(taskScheduleObj *task_obj);
	//��ͣһ���ļ�ֱ��
	void pauseLivingStreamFile(taskScheduleObj *task_obj);
	//ת������RTCP��Ϣ;
	void passLivingRTCPMessage(taskScheduleObj *task_obj);
	//ɾ��һ��RTSP�ͻ���
	void deleteRtspClientSource(taskScheduleObj *task_obj);
	//���һ��ʵʱ��ֱ��
	void startLivingRtspRealtimeStream(taskScheduleObj *task_obj);
	//����RTSP����
	void sendRtspData(taskScheduleObj *task_obj);
	//����json֪ͨ
	void sendJsonNotify(taskScheduleObj *task_obj);
	//�ر�json�׽���
	void closeJsonSocket(taskScheduleObj *task_obj);
	//�����б�;
	std::vector<std::thread>	m_threadLiving;
	volatile bool m_shutdownLiving;
	std::list<taskScheduleObj*>	m_taskLivingScheduleObjList;
	std::mutex	m_mutexLivingObjList;
	//����ִ�е�����;
	std::list<taskExecutor*>		m_taskLivingExecutorList;
	std::mutex	m_mutexLivingExecutorMap;
	std::condition_variable m_scheduleLivingCond;
	//!ֱ������
	///Json���� �迼���̰߳�ȫ
	//�ַ�����ȡ�����ø������߳�,����ͬʱ�������߳����ַ�������ΪJson����ʱ������;
	static void taskJsonDistributionThread(taskSchedule *caller);
	//��������
	void taskJsonHandler(taskScheduleObj *taskObj);
	void parseJsonTask(taskScheduleObj * taskObj);
	void SendGetLivingList(int sockId,char *data,int size);
	void sendCreateTaskReturn(int sockId,std::string &status,std::string &taskId,std::string &userDefString);
	void sendDeleteTaskReturn(int sockId,std::string &status, std::string &taskId, std::string &userDefString);
	void sendSaveFileFailed(int sockId, std::string taskId, std::string fileName, std::string &userDefString);
	//���������б�
	volatile bool m_shutdownJson;
	std::vector<std::thread> m_threadJson;
	std::list<taskScheduleObj*> m_taskJsonScheduleobjList;
	std::mutex m_mutexJsonObjList;
	std::condition_variable m_scheduleJsonCond;
	//�����е������б�
	bool addJsonTask(std::string taskId, taskExecutor* task);
	bool deleteJsonTask(std::string taskId);
	bool deleteAllJsonTask();
	void deleteJsonExecetor(std::vector<taskExecutor*> execetor);
	taskExecutor* getJsonTask(std::string taskId);
	std::map<std::string, taskExecutor*> m_taskJsonExecutorMap;
	std::mutex m_mutexTaskJsonExecutor;
	std::map<int, std::mutex*> m_taskJsonSocketMap;
	std::mutex	m_taskJsonSocketMapMutex;
	//!json����
};


#endif
