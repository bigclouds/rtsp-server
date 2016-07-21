#pragma once

typedef enum
{
	RTCP_SR = 200,		//sender report
	RTCP_RR = 201,		//receiver report
	RTCP_SDES = 202,	//source description
	RTCP_BYE = 203,	//goodbye
	RTCP_APP = 204,	//application defined
}rtcp_pkt_type;

typedef enum
{
	CNAME = 1,
	NAME = 2,
	EMAIL = 3,
	PHONE = 4,
	LOC = 5,
	TOOL = 6,
	NOTE = 7,
	PRIV = 8
}rtcp_info;

typedef struct _RTCP_header
{
#if (BYTE_ORDER==LITTLE_ENDIAN)
	unsigned int rc : 5;		//reception report count
	unsigned int padding : 1;
	unsigned int version : 2;
#elif (BYTE_ORDER==BIG_ENDIAN)
	unsigned int version : 2;
	unsigned int padding : 1;
	unsigned int rc : 5;		//reception report count
#else
#endif
	unsigned int pt : 8;			//packet type
	unsigned int length : 16;
}RTCP_header;

typedef struct _RTCP_header_SR
{
	unsigned int ssrc;	//RTCP SSRC;
	unsigned int ntp_timestamp_MSW;	//MSW
	unsigned int ntp_timestamp_LSW;	//LSW
	unsigned int rtp_timestamp;
	unsigned int pkt_count;
	unsigned int octet_count;
}RTCP_header_SR;

typedef struct _RTCP_header_RR
{
	unsigned int ssrc;
}RTCP_header_RR;

typedef struct _RTCP_header_SR_report_block
{
	unsigned int	ssrc;		//SSRC of RTP session;
	unsigned char	fract_lost;//RR 0;
	unsigned char	pck_lost[3];//24bit lost rtp;
	unsigned int	h_seq_no;//low 16bit max rtp ssrc,16 bit extern;
	unsigned int	jitter;//RTP ���ݱ���interarrival ʱ���ͳ�Ʒ���Ĺ�ֵ
	unsigned int	last_SR;//�����SRʱ���
	unsigned int	delay_last_SR;//�ӽ��յ���ԴSSRC_n ������һ��SR �����ͱ����ձ����ļ������ʾΪ1/65536 ��һ����Ԫ�������δ�յ�SR��DLSR ������Ϊ��
}RTCP_header_SR_report_block;

typedef struct _RTCP_header_SDES
{
	unsigned int	ssrc;
	unsigned char	attr_name;
	unsigned char	len;
}RTCP_header_SDES;

typedef struct _RTCP_header_BYE
{
	unsigned int	ssrc;
	unsigned char	length;
}RTCP_header_BYE;

//����RR��ʱ��Ҫ�Ĳ���,����Ϊ�˼����β��б�;
typedef struct _RTCP_RR_generate_param
{

}RTCP_RR_generate_param;