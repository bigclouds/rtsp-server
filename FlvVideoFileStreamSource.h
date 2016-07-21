#pragma once
#include "streamSource.h"
#include "task_schedule_obj.h"
#include "RTSP.h"
#include "flvParser.h"
#include "flvIdx.h"
#include <list>
#include <vector>
#include <thread>
#include <mutex>

#include <condition_variable>
//play Ҫע�⿪ʼʱ�䣬
//����ʱҪע��rtpʱ�䣬�����þ���ʱ��
//pause��ɾ��
//������һ֡��ʱ��������ʱ�������������fps
class FlvVideoFileStreamSource :
	public streamSource
{
public:
	FlvVideoFileStreamSource(const char *file_name, LP_RTSP_session session);
	virtual ~FlvVideoFileStreamSource();
	virtual int handTask(taskScheduleObj *task_obj);
protected:
	virtual int startStream(taskScheduleObj *task_obj);
	virtual int stopStream(taskScheduleObj *task_obj);
	virtual int notify_stoped();
	//�����߳�;
	void sendThread();
	unsigned short incrementSeq();
	//���RTP_header��һ��buffer��;
	void fillRtpHeader2Buf(RTP_header &rtp_header, char *buf,unsigned int timeFrame);
	void shutdown();
	//��ȡsps,pps,������ߺ�fps;
	void initStreamFile();
	//����֡���ɹ�����0
	int sendFrame(timedPacket &packetSend);
	//����timedpacket���ɿɷ���rtp֡���ɹ�����0
	int generateSendableFrame(std::list<DataPacket> &frames, timedPacket &packetSend, int  frame_size);
	//seek ��ָ��ʱ��
	int seekToTime(double targetTimeS);
	std::list<DataPacket>	m_lastKeyframe;
	bool m_fileInited;
	bool m_IdxExist;
	bool m_hasCalledDelete;
	flvParser		*m_flvParser;
	flvIdxReader	*m_flvIdxReader;
	LP_RTSP_session		m_session;
	unsigned int m_lastRtpTime;
	std::thread	m_sendThread;
	std::mutex	m_mutexSend;
	bool m_sendEnd;
	in_addr m_sock_addr;
	int m_RTSP_socket_id;
	int m_rtp_socket;
	int m_rtcp_socket;
	double	m_startTime;
	double	m_endTime;
	std::mutex	m_mutexRtspSend;
	std::vector<std::string>	m_vectorRtspSendData;
	unsigned int m_rtcp_ssrc;
	DataPacket m_sps;
	DataPacket m_pps;
	timeval m_rtcp_time;
	unsigned int m_rtp_pkg_counts;
	unsigned int m_rtp_data_size;

	int m_seq;
	int m_seq_num;
};

