#pragma once
#include <list>
#include<map>
#include <mutex>
#include "RTP.h"	//for datapacket

const unsigned int TCP_DEFAULT_CACHER_SIZE = 4096;

//�����������buffer������Ƿ������Ҫ��buffer���趪�����ֽ���
typedef void(*tcpBufAnalyzer)(char* dataIn,unsigned int inSize,bool &getValid,
	unsigned int &outStart, unsigned int &outEnd, unsigned int& invalidBytes);
//����RTP RTCP RTSP ����
void parseRtpRtcpRtspData(char* dataIn, unsigned int inSize, bool &getValid,
	unsigned int &outStart, unsigned int &outEnd,unsigned int& invalidBytes);
//����size+data��taskJson
void parseTaskJsonData(char* dataIn, unsigned int inSize, bool &getValid,
	unsigned int &outStart, unsigned int &outEnd, unsigned int&invalidBytes);

//����TCP����ʽ�׽��ִ������軺���buffer

class tcpCacher
{
public:
	tcpCacher(unsigned int maxBufSize= TCP_DEFAULT_CACHER_SIZE);
	virtual ~tcpCacher();
	//���buffer,�ɹ�����0;
	int appendBuffer(char *dataIn, unsigned int size);
	//��ȡbuffer,�ɹ�����0;
	int getBuffer(char *dataOut, unsigned int size, bool remove=true);
	//��ȡ��Чbuffer���Ƴ���Ч�ֽ�,�ɹ�����0;
	int getValidBuffer(std::list<DataPacket>& dataList,tcpBufAnalyzer analyzer);
private:
	tcpCacher(tcpCacher&)=delete;
	tcpCacher& operator=(tcpCacher)=delete;

	//�����С
	unsigned int m_MaxBufSize;
	//��������
	char *m_dataBuf;
	//��Ч���ݴ�С
	unsigned int m_dataSize;
	//��Ч�������
	unsigned int m_cur;
	std::mutex m_dataMutex;
};

typedef std::map<unsigned int, tcpCacher*> tcpCacherMap;

class tcpCacherManager
{
public:
	tcpCacherManager();
	virtual ~tcpCacherManager();
	//���buffer,�ɹ�����0;
	int appendBuffer(unsigned int sockId, char *dataIn, unsigned int size);
	//��ȡbuffer,�ɹ�����0;
	int getBuffer(unsigned int sockId, char *dataOut, unsigned int size, bool remove = true);
	//��ȡ��Чbuffer���Ƴ���Ч�ֽ�,�ɹ�����0;
	int getValidBuffer(unsigned int sockId, std::list<DataPacket>& dataList, tcpBufAnalyzer analyzer);
	//�Ƴ��ر��˵�tcp,�ɹ�����0;
	void removeCacher(unsigned int sockId);
private:
	tcpCacherManager(tcpCacherManager&);
	tcpCacherManager& operator=(tcpCacherManager)=delete;

	tcpCacherMap	m_tcpCacherMap;
	tcpCacherMap::iterator m_Iterator;
};
