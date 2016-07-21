#include "RTSP_server.h"
#include "taskSendRtspResbData.h"
#include "taskSchedule.h"

int RTSP_state_machine(LP_RTSP_buffer rtsp,RTSP_CMD_STATE method)
{
	int iRet=0;
	do
	{
		char	*s;
		LP_RTSP_session	pSession=nullptr;
		char session_id[64];
		char	trash[256];
		//�ǺϿ���:
		//һ��track��Ӧһ��session��һ��RTSP���ӿ�ͬʱ���session;
		//setup��������Ӧ��session,ssrc ���ն˸���SSRC���ж�����Ƶ������Ƶ;
		//play�������������Ѿ�SETUP�ɹ���track;
		//�Ͽ���:
		/*����ǰ���£��������п��Ա�����һ��������ƵĶ������
		�ڴ�����£��ͼ������ڸ�����URL��ý�弶"a=control:"���ԣ����������Ͽ��Ƶ�����URL�ĻỰ��"a=control:"��
		���ý�弶URL�����URL��������������C1.1������������Ϊ����URL��
		�����ʾֻ�ɵ�������ɣ�ý�弶"a=control:"����ȫ�����ԡ�
		���ǣ������ʾ��������һ������ÿ��ý�������֡����롿�����Լ���"a=control:"���ԡ�*/
		//a=control:rtsp://example.com/movie/
		//��session
		if ((s=strstr(rtsp->in_buffer,HDR_SESSION))!=0)
		{
			if (sscanf(s,"%254s %s",trash,&session_id)!=2)
			{
				RTSP_send_err_reply(454,0,rtsp);
				iRet=-1;
				break;
			}
			std::list<LP_RTSP_session>::iterator it;
			for (it=rtsp->session_list.begin();it!=rtsp->session_list.end(); it++)
			{
				if (strcmp(session_id,(*it)->session_id.c_str())==0)
				{
					pSession = *it;
					break;
				}
			}
		}else
		{
			//û��session,����Ĭ��session;
			pSession=*rtsp->session_list.begin();
		}
		if (pSession==nullptr)
		{
			pSession = *rtsp->session_list.begin();
		}

		char reply[RTSP_BUFFER_SIZE];
		taskSendRtspResbData *sendTask = nullptr;
		switch (pSession->cur_state)
		{
		case INIT_STAT:
			switch (method)
			{
			case RTSP_ID_DESCRIBE:
				iRet=RTSP_DESCRIBE(rtsp);
				break;
			case RTSP_ID_SETUP:
				iRet=RTSP_SETUP(rtsp,pSession);
				if (iRet==0)
				{
					pSession->cur_state=READY_STAT;
				}
				break;
			case RTSP_ID_TEARDOWN:
				iRet=RTSP_TEARDOWN(rtsp,pSession);
				break;
			case RTSP_ID_OPTIONS:
				iRet=RTSP_OPTIONS(rtsp);
				if (iRet==0)
				{
					pSession->cur_state=INIT_STAT;
				}
				break;
			case RTSP_ID_PLAY:
			case RTSP_ID_PAUSE:
				RTSP_send_err_reply(455, "Accept: OPTIONS, DESCRIBE, SETUP, TEARDOWN\n", rtsp);
				break;
			default:
				RTSP_send_err_reply(501, "Accept: OPTIONS, DESCRIBE, SETUP, TEARDOWN\n", rtsp);
				break;
			}
			break;
		case READY_STAT:
			switch(method)
			{
			case RTSP_ID_PLAY:
				iRet=RTSP_PLAY(rtsp,pSession);
				if (iRet==0)
				{
					pSession->cur_state=PLAY_STAT;
				}
				break;
			case RTSP_ID_SETUP:
				iRet = RTSP_SETUP(rtsp, pSession);
				if (iRet == 0)
				{
					pSession->cur_state = READY_STAT;
				}
				break;
			case RTSP_ID_TEARDOWN:
				iRet = RTSP_TEARDOWN(rtsp, pSession);
				break;
			default:
				RTSP_send_err_reply(501, "Accept: OPTIONS, SETUP, PLAY, TEARDOWN\n", rtsp);
				break;
			}break;
		case PLAY_STAT:
			switch (method)
			{
			case RTSP_ID_BAD_REQ:
				break;
			case RTSP_ID_DESCRIBE:
				break;
			case RTSP_ID_ANNOUNCE:
				break;
			case RTSP_ID_GET_PARAMETERS:
				break;
			case RTSP_ID_OPTIONS:
				//�������ڲ���״̬�����﷢��socket�ܿ��ܻ������streamSource����ȥ��Ӧ
				//RTSP_send_err_reply(551, "Accept: PAUSE, TEARDOWN\n", rtsp);
				sprintf(reply, "%s %d %s" RTSP_EL"CSeq: %d", RTSP_VER, 551, get_stat(551), rtsp->rtsp_seq);
				strcat(reply, RTSP_EL RTSP_EL);
				sendTask = new taskSendRtspResbData(reply, rtsp->session_list.front()->session_id);
				taskSchedule::getInstance().addTaskObj(sendTask);
				break;
			case RTSP_ID_PAUSE:
				LOG_INFO(configData::getInstance().get_loggerId(),"RTSP pause in play state");
				iRet = RTSP_PAUSE(rtsp,pSession);
				if (iRet==0)
				{
					pSession->cur_state = READY_STAT;
				}
				break;
			case RTSP_ID_PLAY:
				break;
			case RTSP_ID_RECORD:
				break;
			case RTSP_ID_REDIRECT:
				break;
			case RTSP_ID_SETUP:
				break;
			case RTSP_ID_SET_PARAMETER:
				break;
			case RTSP_ID_TEARDOWN:
				iRet = RTSP_TEARDOWN(rtsp, pSession);
				break;
			default:
				break;
			}
		default:
			break;
		}
	} while (0);
	return iRet;
}
