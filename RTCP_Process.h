#pragma once

#include "RTCP.h"
#include "RTP.h"
#include "BitReader.h"



class RTCP_process
{
public:
	RTCP_process();
	RTCP_process(RTCP_process&);
	~RTCP_process();

	//����RTCP,�ɹ�����0;
	int parse_RTCP_buf(const DataPacket &data_packet);
	//����RTCP SR,�ɹ�����0;
	static int generate_RTCP_SR_buf(DataPacket &data_packet,unsigned int SSRC,unsigned int packet_count,unsigned int octet_count);
	//����RTCP RR,�ɹ�����0;
	static int generate_RTCP_RR_buf(DataPacket &data_packet,RTCP_RR_generate_param param);
	//����RTCP bye,�ɹ�����0;
	static int generate_Bye_Buf(DataPacket &data_packet, unsigned int SSRC);
	//����RTCP SDES���ɹ�����0;
	static int generate_SDES_Buf(DataPacket &data_packet, unsigned int SSRC);
	//���ظ�RTCP��SSRC;
	unsigned int RTCP_SSRC();
	//��Ч��report block����;
	int report_block_count();
	//���ؿͻ��˷��ͷ����߱���RTP��SSRC;
	unsigned int RTP_ssrc(int index);
private:
	int	parseSR();
	int parseRR();
	int parseSDES();
	int	parseBYE();
	int parseAPP();
	static const char* get_CNAME(int &len);

	rtcp_pkt_type	m_pkt_type;
	RTCP_header		m_RTCP_header;
	RTCP_header_SR	m_RTCP_header_SR;
	RTCP_header_RR	m_RTCP_header_RR;
	RTCP_header_SR_report_block*	m_pRTCP_report_block;
	RTCP_header_SDES*	m_pRTCP_SDES;
	int				m_count_SDES;
	RTCP_header_BYE	m_RTCP_goodbye;

	CBitReader	m_BitReader;
};

