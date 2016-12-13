#pragma once
#include "../pluginmain.h"

class Expression 
{
protected:
	// ��ȡ�����ڴ�
	SSIZE_T readProcMemValue(void* lpAddr , unsigned int uSize);
	// ��ȡ�̼߳Ĵ�����ֵ
	bool    readRegValue(const char* pReg , const char** pEnd , SSIZE_T& uRegValue);
	bool	WriteRegValue(const char* pReg , const char** pEnd , SSIZE_T& uRegValue);

	// ��ȡ���ʽ���յ�ֵ
	bool    Value(SSIZE_T& uValue , const char* pExpression , const char** pEnd , int nPriorty);

public:
	Expression();
	~Expression();

	SSIZE_T Value(const char* pExpression);
};

