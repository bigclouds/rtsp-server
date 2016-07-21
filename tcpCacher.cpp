#include "tcpCacher.h"
#include "configData.h"
#include "RTSP_server.h"
#include "taskJsonStructs.h"
#include <mutex>

tcpCacher::tcpCacher(unsigned int maxBufSize):m_MaxBufSize(maxBufSize),m_dataBuf(0),m_dataSize(0),m_cur(0)
{
	m_dataBuf = (char*)malloc(m_MaxBufSize);
}

tcpCacher::~tcpCacher()
{
	std::lock_guard<std::mutex> guard(m_dataMutex);
	free(m_dataBuf);
}

int tcpCacher::appendBuffer(char * dataIn, unsigned int size)
{
	std::lock_guard<std::mutex> guard(m_dataMutex);
	//�Ƿ��Խ��
	unsigned int newEnd = m_cur + m_dataSize + size;
	if (newEnd>=m_MaxBufSize)
	{
		//�ܴ�С�ǲ��ǳ��������С
		unsigned int nowSize = m_dataSize + size;
		if (nowSize>m_MaxBufSize)
		{
			//���������·���ռ�,�����������Ч
			if (nowSize>=0xfffff)
			{
				LOG_WARN(configData::getInstance().get_loggerId(), "too big tcp data?" << size);
				return -1;
			}
			m_MaxBufSize = nowSize;
			m_dataBuf = (char*)realloc(m_dataBuf, m_MaxBufSize);
		}
		//��������λ��
		if (m_cur>0)
		{
			memmove(m_dataBuf, m_dataBuf + m_cur, m_dataSize);
			m_cur = 0;
		}
	}
	//׷������
	int curEnd = m_cur + m_dataSize;
	memcpy(m_dataBuf + curEnd, dataIn, size);
	m_dataSize += size;
	return 0;
}

int tcpCacher::getBuffer(char * dataOut, unsigned int size, bool remove)
{
	std::lock_guard<std::mutex> guard(m_dataMutex);
	int ret = 0;
	do
	{
		if (size<=0||size>m_dataSize)
		{
			ret = -1;
			break;
		}
		//__try
		{
			if (dataOut)
			{
				memcpy(dataOut, m_dataBuf + m_cur, size);
			}

			if (remove)
			{
				m_cur += size;
				m_dataSize -= size;
			}
		}
		//__except (EXCEPTION_EXECUTE_HANDLER)
		//{
		//	LOG_ERROR(configData::getInstance().get_loggerId(), "");
//
		//}
	} while (0);
	return ret;
}

int tcpCacher::getValidBuffer(std::list<DataPacket>& dataList, tcpBufAnalyzer analyzer)
{
	//std::lock_guard<std::mutex> guard(m_dataMutex);
	m_dataMutex.lock();
	int ret = 0;
	std::list<DataPacket>::iterator it;
	for (it=dataList.begin(); it!=dataList.end(); it++)
	{
		if (it->data)
		{
			delete[]it->data;
			it->data = 0;
		}
	}
	dataList.clear();
	do
	{
		bool getValid = false;
		unsigned int outStart = 0;
		unsigned int outEnd = 0;
		unsigned int invalidBytes = 0;
		if (m_dataSize<=0)
		{
			break;
		}
		DataPacket dataPacket;
		analyzer(m_dataBuf + m_cur, m_dataSize, getValid, outStart, outEnd, invalidBytes);
		if (!getValid||outEnd>m_dataSize)
		{
			m_dataMutex.unlock();
			getBuffer(0, invalidBytes, true);
			m_dataMutex.lock();
			break;
		}
		//ȡ���ݷ���List
		dataPacket.size = outEnd - outStart + 1;
		dataPacket.data = new char[dataPacket.size];
		memcpy(dataPacket.data, m_dataBuf + outStart + m_cur, dataPacket.size);
		dataList.push_back(dataPacket);
		//�ƶ�m_cur������size
		m_cur += outEnd+1;
		m_dataSize -= outEnd+1;
	} while (true);
	m_dataMutex.unlock();
	
	return ret;
}

void parseRtpRtcpRtspData(char* dataIn, unsigned int inSize, bool &getValid,
	unsigned int &outStart, unsigned int &outEnd, unsigned int& invalidBytes)
{
	outStart = 0;
	outEnd = 0;
	bool getStart = false;
	invalidBytes = 0;
	getValid = false;
	while (true)
	{
		if (outStart>=inSize||outEnd>=inSize)
		{
			break;
		}
		//����Ч��ͷ
		if (getStart == false)
		{
			//Ѱ��$��R
			if (dataIn[outStart] == '$')
			{
				if (inSize-outStart<4)
				{
					getValid = false;
					break;
				}
				else
				{
					outEnd = outStart + 1;
					getStart = true;
				}
			}
			/*else if (dataIn[outStart] == 'R')
			{

				if (inSize-outStart<8)
				{
					getValid = false;
					break;
				}
				else if (strncmp(dataIn+outStart,"RTSP",4)==0)
				{
					outEnd = outStart + 1;
				}
				else
				{
					outStart++;
					invalidBytes++;
					continue;
				}
			}*/
			else
			{
				outEnd = outStart + 1;
				getStart = true;
				continue;
			}

		}
		else
		{
			if (dataIn[outStart]=='$')
			{
				char size1, size2;
				char channel;
				channel = dataIn[outStart + 1];
				size1 = dataIn[outStart + 2];
				size2 = dataIn[outStart + 3];
				unsigned short pkg_size = ((size1 << 8) | size2);
				//pkg_size���̫��Ҳ����Ч�ģ������Ҳ��0xFFFF�����ÿ��ǣ�
				//RTSPָ���迼�ǣ����̫��û���ֽ�������ɾ����Ч�ַ���������æ�Ƚ�����
				//δ�յ�������RTP data
				if (inSize-outStart<pkg_size+4)
				{
					break;
				}
				//-1��Ϊ�±�0��ʼ��outEnd����size����λ��
				outEnd = outStart + pkg_size + 4 - 1;
				getValid = true;
				break;
			}
			else
			{
				//��ֹ�����յ���������ݣ�һֱû�н���������������buffer size;
				unsigned int tmpEnd = inSize;
				if (inSize-outEnd>RTSP_BUFFER_SIZE)
				{
					tmpEnd = outEnd = RTSP_BUFFER_SIZE;
				}

				for (unsigned int i = outEnd; i < tmpEnd-3; i++)
				{
					if (strncmp(dataIn+i,"\r\n\r\n",4)==0)
					{
						outEnd = i+3;
						getValid = true;
					}
				}
				break;
			}
		}
	}
}

void parseTaskJsonData(char * dataIn, unsigned int inSize, bool & getValid, unsigned int & outStart, unsigned int & outEnd, unsigned int & invalidBytes)
{
	outStart = 0;
	outEnd = 0;
	invalidBytes = 0;
	getValid = false;
	while (true)
	{
		if (outStart >= inSize || outEnd >= inSize)
		{
			break;
		}
		//size
		if (outStart+4>=inSize)
		{
			getValid = false;
			break;
		}
		unsigned int dataSize=htonl((unsigned int)(*((unsigned int*)(dataIn + outStart))));
		memcpy(&dataSize, dataIn + outStart, 4);
		dataSize = htonl(dataSize);
		if (dataSize>MAX_JSON_SIZE)
		{
			LOG_WARN(configData::getInstance().get_loggerId(), "bad Json Size" << dataSize<<"\tlast size:"<<inSize-outStart);
			invalidBytes++;
			outStart++;
			continue;
		}
		if (outStart + 5 < inSize)
		{
			char data4 = dataIn[outStart + 5];
			if (*(dataIn + outStart + 4) != '{')
			{
				invalidBytes++;
				outStart++;
				continue;
			}
		}
		//data
		outEnd = outStart + sizeof(dataSize) + dataSize - 1;
		if (outEnd >= inSize)
		{
			
			break;
		}
		getValid = true;
		break;
	}
}
