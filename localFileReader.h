#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int CACHE_SIZE_DEFAULT=1024*1024*4;

class localFileReader
{
public:
	localFileReader(const char *file_name,int cache_size=CACHE_SIZE_DEFAULT);
	~localFileReader();
	//�ôζ�ȡʱ���ļ��ܴ�С;
	long fileSize();
	//��ǰ��ȡ����;
	long fileCur();
	//��ȡ�ļ�,�ɹ�����0;�ļ�����-1;���治������-2;�ļ�������-3;
	int readFile(char *out_buf,int  &read_len,bool remove=true);
	//seek�ļ�,�ɹ�����0;
	int seekFile(long pos);
	//���˼����ֽ�;
	int backword(long bytes);
protected:
	localFileReader(localFileReader&);
private:
	int initFileReader(const char *file_name);
	long getFileSize(FILE *fp);
	//�ɹ�����0;
	int seekToPos(long pos);
	long	m_file_size;
	//��ǰcache������ļ������;
	long m_cur_cache_start_pos;
	//������ǰcache��λ�ã���0��ʼ;
	long m_cur;
	char *m_file_cache;
	int m_cache_size;
	int m_valid_cache_size;
	FILE *m_fp;
	char *m_file_name;
};
