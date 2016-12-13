#pragma once
#include "../pluginmain.h"

class Expression 
{
protected:
	// 读取进程内存
	SSIZE_T readProcMemValue(void* lpAddr , unsigned int uSize);
	// 获取线程寄存器的值
	bool    readRegValue(const char* pReg , const char** pEnd , SSIZE_T& uRegValue);
	bool	WriteRegValue(const char* pReg , const char** pEnd , SSIZE_T& uRegValue);

	// 获取表达式最终的值
	bool    Value(SSIZE_T& uValue , const char* pExpression , const char** pEnd , int nPriorty);

public:
	Expression();
	~Expression();

	SSIZE_T Value(const char* pExpression);
};

