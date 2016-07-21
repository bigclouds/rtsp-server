#pragma once
#include <string>
#include "fileHander.h"
//���ļ���ͷΪƫ������ʼλ��
//��һ��ÿ���ؼ�֡ǰ����sps pps �Թؼ�֡λ��Ϊ��
//64λdouble��Ϊ��λ,64λlong longΪƫ����
//�����ֽ���

static const std::string idxStuff = ".Idx";

struct flvIdx
{
	double time;
	unsigned long long offset;
};

class flvIdxWriter
{
public:
	flvIdxWriter();
	virtual ~flvIdxWriter();
	bool initWriter(std::string fileName);
	bool appendIdx(flvIdx &idx);
private:
	fileHander *m_fileHander;
	bool m_inited;
};

class flvIdxReader
{
public:
	flvIdxReader();
	~flvIdxReader();
	bool initReader(std::string fileName);
	bool getNearestIdx(double time, flvIdx &idx);
private:
	void resetCur();
	fileHander *m_fileHander;
	bool m_inited;
};

