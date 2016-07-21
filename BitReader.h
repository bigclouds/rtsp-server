#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

class CBitReader
{
public:
	CBitReader(void);
	~CBitReader(void);
	//�ɹ�����0
	int SetBitReader(unsigned char* Buf,int len,bool useRef=false);
	unsigned int ReadBit();
	unsigned int ReadBits(int nBits);
	unsigned int Read32Bits();
	int	CurrentBitCount();
	int CurrentByteCount();
	unsigned char* CurrentByteData();
	void	IgnoreBytes(int nBytesIgnore);
	unsigned int ReadExponentialGolombCode();
	unsigned int ReadSE();
	unsigned char*	m_buf;
private:
	CBitReader(CBitReader&);
	bool		m_ref;	//�½���������ֱ������
	int			m_curBit;
	int			m_TotalByte;
};

