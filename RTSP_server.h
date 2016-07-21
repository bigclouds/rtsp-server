#ifndef RTSP_SERVER_H_
#define RTSP_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "socketEvent.h"
#include "RTSP.h"
#include "util.h"
#include "RTP.h"
#include "configData.h"

int create_RTSP_server(char *hostName,short port,socketEventCallback callback);
void createTaskJsonServer(short port,socketEventCallback callback);
void RTSP_Callback(socket_event_t	*data);
void RTP_Callback(socket_event_t *data);
void RTCP_Callback(socket_event_t *data);
void taskJsonCallback(socket_event_t *data);

//ʹ��UDP��ʽʱ����Ҫ�����˿ڽ��տͻ��˷�����RTP��RTCP���ݡ�
int	 create_RTP_RTCP_server(socketEventCallback RTP_callback,socketEventCallback RTCP_callback);
unsigned short  Get_RTP_Port();
unsigned short	Get_RTCP_Port();

//��ȡ��ǰsocket��RTSPbuffer�����û�����½�һ��;
LP_RTSP_buffer Get_RTSP_buffer(socket_t const	sock_id);
//����RTSP��Ϣ;�ɹ�����0;
int	RTSP_handler(LP_RTSP_buffer rtsp);
//����TCP����;�ɹ�����0;
int TCP_data_parse_valid_RTSP_RTP_RTCP_data(LP_RTSP_buffer rtsp,std::list<DataPacket> &data_list);
//�ж�RTSPbuffer�Ƿ��������;�ɹ�����0;
int RTSP_end(LP_RTSP_buffer rtsp);
//�ͻ���socket�ѶϿ�,�ɹ�����0;
int RTSP_Close_Client(socket_t sock_id);
//�Ƿ�Ϊ��Ч��RTSP��Ϣ,�ɹ�����RTSPָ������;
RTSP_CMD_STATE RTSP_validate_method(LP_RTSP_buffer rtsp);
//�Ƿ�ΪRTCP��Ϣ;�ɹ�����0;
int is_RTCP_data(LP_RTSP_buffer rtsp);
//����RTSP״̬��method������Ҫ���ص�buffer
int RTSP_state_machine(LP_RTSP_buffer rtsp,RTSP_CMD_STATE method);
//����RTSP������Ϣ
int RTSP_send_err_reply(int errCode,char *addon,LP_RTSP_buffer rtsp);
//����������ӿͻ����յ���OPTIONS,�ɹ�����0;
int RTSP_OPTIONS(LP_RTSP_buffer rtsp);
//��ͻ��˷���OPTIONS����Ӧ��Ϣ,�ɹ�����0;
int RTSP_OPTIONS_reply(LP_RTSP_buffer rtsp);
//����������ӿͻ����յ���DESCRIBE���ɹ�����0;
int RTSP_DESCRIBE(LP_RTSP_buffer rtsp);
//��ͻ��˷���DESCRIBE����Ӧ��Ϣ���ɹ�����0;
int RTSP_DESCRIBE_reply(LP_RTSP_buffer rtsp,char	*object,char *descr);
//������buffer��RTSP_buffer,�ɹ�����0;
int RTSP_add_out_buffer(char *buffer,unsigned short len,LP_RTSP_buffer rtsp);
//���ݴ����뷵�ش����ַ���;
char *get_stat(int err);
//���RTSP ʱ��;
void add_RTSP_timestamp(char *p);
//����url���ɹ�����0����Ч����1��������󷵻�-1;
int parse_url(const char *url,char *server,size_t server_len,unsigned short &port,char *filename,size_t filename_len);
//�ж��Ƿ�Ϊ֧�ֵ��ļ�����,�ɹ�����0:video 1:audio;
int is_supported_file_stuffix(char *p);
//����SDP��Ϣ,�ɹ�����0;
int generate_SDP_description(char *url,char *descr);
//�����ض���ʽ����SDP��Ϣ����generate_SDP_descriptionʹ��;
int generate_SDP_info(char *url,char *sdpLines,size_t sdp_line_size);
//����������SETUP��Ϣ,�ɹ�����0;
int RTSP_SETUP(LP_RTSP_buffer rtsp,LP_RTSP_session &session);
//��ͻ��˷���SETUP��Ӧ,�ɹ�����0;
int RTSP_SETUP_reply(LP_RTSP_buffer rtsp,LP_RTSP_session session);
//�ر�һ��RTSP����,�ɹ�����0;
int RTSP_TEARDOWN(LP_RTSP_buffer rtsp, LP_RTSP_session session);
//����TEARDOWN��Ӧ,�ɹ�����0;
int RTSP_TEARDOWN_reply(LP_RTSP_buffer rtsp, LP_RTSP_session session);
//����������PLAY��Ϣ���ɹ�����0;
int RTSP_PLAY(LP_RTSP_buffer rtsp,LP_RTSP_session session);
//����PLAY��Ӧ���ɹ�����0;
int RTSP_PLAY_reply(LP_RTSP_buffer rtsp,char *object,LP_RTSP_session session);
//����������pause��Ϣ���ɹ�����0;
int RTSP_PAUSE(LP_RTSP_buffer rtsp,LP_RTSP_session session);
//����pause��Ӧ���ɹ�����0;
int RTSP_PAUSE_reply(LP_RTSP_buffer rtsp, LP_RTSP_session session);

//���ݵ�ǰʱ�����������RTPʱ�䣬��video9000��audio8000;
unsigned int convert_to_RTP_timestamp(timeval tv, int freq);

//taskJson
void taskJsonCloseClient(socket_t sockId);
#endif