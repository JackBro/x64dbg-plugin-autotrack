#include "Expression.h"
#include "../pluginmain.h"

Expression::Expression()
{

}


Expression::~Expression()
{
}



/*
  : ����BYTE/WORD/DWORD/QWORD [ ʱ,�Ȼ�ȡ[]���ʽ�е�ֵ
	�����Ĵ���,��ȡ�Ĵ�����ֵ.
���ȼ���: 0~9, ��ֵԽ�����ȼ�Խ��.
	ÿ�κ����ĵ��ö��ᴫ��һ�����ȼ���ֵ,�����ȼ����߼���֧
	���ܴ�������ȼ�����Ĳ���
*/
const char *skipspace(const char *p)
{
	for(; *p == ' ' || *p == '\t'; p++);
	return p;
}


bool isReg(const char* pStr , const char** pEnd)
{
	if(*pStr == 'e')
	{
		switch(*((WORD*)(pStr + 1)))
		{
			case 'xa'://eax
			case 'xc'://ecx
			case 'xd'://edx
			case 'xb'://ebx
			case 'is'://esi
			case 'id'://edi
			case 'ps'://esp
			case 'pb'://ebp
			case 'pi'://eip
				if(pEnd)
					*pEnd = pStr + 3;
				return true;
		}
	}
	else
	{
		switch(*((WORD*)(pStr + 1)))
		{
			case 'xa'://ax
			case 'cx'://cx
			case 'dx'://dx
			case 'bx'://bx
			case 'si'://si
			case 'di'://di
			case 'sp'://sp
			case 'bp'://bp
			case 'la'://al
			case 'ha'://ah
			case 'lc'://cl
			case 'hc'://ch
			case 'ld'://dl
			case 'hd'://dh
			case 'lb'://bl
			case 'hb'://bh
				if(pEnd)
					*pEnd = pStr + 2;
				return true;
		}
	}
	return false;
}

SSIZE_T Expression::readProcMemValue(LPVOID lpAddr , unsigned int dwSize)
{
	SSIZE_T nValue = 0;
	DbgMemRead((duint)lpAddr , (unsigned char*)&nValue , dwSize);
	return nValue;
}


bool Expression::readRegValue(const char* pReg ,const char** pEnd, SSIZE_T& uRegValue)
{
	if(pReg == NULL)
		return false;
	

	pReg = skipspace(pReg);
	if(*pReg == 'e')
	{
		*pEnd = pReg + 3;
		switch(*((WORD*)(pReg + 1)))
		{
			case 'xa'://eax
				uRegValue = GetContextData(UE_EAX);
				break;
			case 'xc'://ecx
				uRegValue = GetContextData(UE_ECX);
				break;
			case 'xd'://edx
				uRegValue = GetContextData(UE_EDX);
				break;
			case 'xb'://ebx
				uRegValue = GetContextData(UE_EBX);
				break;
			case 'is'://esi
				uRegValue = GetContextData(UE_ESI);
				break;
			case 'id'://edi
				uRegValue = GetContextData(UE_EDI);
				break;
			case 'ps'://esp
				uRegValue = GetContextData(UE_ESP);
				break;
			case 'pb'://ebp
				uRegValue = GetContextData(UE_EBP);
				break;
			case 'pi':// eip
				uRegValue = GetContextData(UE_EIP);
				break;
			default:
				uRegValue = 0;
				return false;
		}
	}
	else
	{
		*pEnd = pReg + 2;
		switch(*((WORD*)(pReg + 1)))
		{
			case 'xa'://ax
				uRegValue = GetContextData(UE_EAX) & 0xFFFF;
				break;
			case 'xc'://cx
				uRegValue = GetContextData(UE_ECX) & 0xFFFF;
				break;
			case 'xd'://dx
				uRegValue = GetContextData(UE_EDX) & 0xFFFF;
				break;
			case 'xb'://bx
				uRegValue = GetContextData(UE_EBX) & 0xFFFF;
				break;
			case 'is'://si
				uRegValue = GetContextData(UE_ESI) & 0xFFFF;
				break;
			case 'id'://di
				uRegValue = GetContextData(UE_EDI) & 0xFFFF;
				break;
			case 'ps'://sp
				uRegValue = GetContextData(UE_ESP) & 0xFFFF;
				break;
			case 'pb'://bp
				uRegValue = GetContextData(UE_EBP) & 0xFFFF;
				break;
			case 'la'://al
				uRegValue = GetContextData(UE_EAX) & 0x0f;
				break;
			case 'ha'://ah
				uRegValue = GetContextData(UE_EAX) & 0xf0;
				break;
			case 'lc'://cl
				uRegValue = GetContextData(UE_ECX) & 0x0f;
				break;
			case 'hc'://ch
				uRegValue = GetContextData(UE_ECX) & 0xf0;
				break;
			case 'ld'://dl
				uRegValue = GetContextData(UE_EDX) & 0x0f;
				break;
			case 'hd'://dh
				uRegValue = GetContextData(UE_EDX) & 0xf0;
				break;
			case 'lb'://bl
				uRegValue = GetContextData(UE_EBX) & 0x0f;
				break;
			case 'hb'://bh
				uRegValue = GetContextData(UE_EBX) & 0xf0;
				break;
			default:
				uRegValue = 0;
				return false;
		}
	}
	return true;
}

bool Expression::WriteRegValue(const char* pReg , const char** pEnd , SSIZE_T& uRegValue)
{
	if(pReg == NULL)
		return false;
	CONTEXT ct = { 0 };
	ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
// 
// 	if(!m_pDdbgObj->GetRegInfo(ct))
// 		return false;

	pReg = skipspace(pReg);
	if(*pReg == 'e')
	{
		*pEnd = pReg + 3;
		switch(*((WORD*)(pReg + 1)))
		{
			case 'xa'://eax
				SetContextData(UE_EAX , uRegValue);
				break;
			case 'xc'://ecx
				SetContextData(UE_ECX , uRegValue);
				break;
			case 'xd'://edx
				SetContextData(UE_EDX , uRegValue);
				break;
			case 'xb'://ebx
				SetContextData(UE_EBX , uRegValue);
				break;
			case 'is'://esi
				SetContextData(UE_ESI, uRegValue);
				break;
			case 'id'://edi
				SetContextData(UE_EDI , uRegValue);
				break;
			case 'ps'://esp
				SetContextData(UE_ESP, uRegValue);
				break;
			case 'pb'://ebp
				SetContextData(UE_EBP , uRegValue);
				break;
			case 'pi':// eip
				SetContextData(UE_EIP , uRegValue);
				break;
			default:
				 0;
				return false;
		}
	}
	else
	{
		*pEnd = pReg + 2;
		switch(*((WORD*)(pReg + 1)))
		{
			case 'xa'://ax
				SetContextData(UE_EAX , uRegValue & 0xFFFF);
				break;
			case 'xc'://cx
				SetContextData(UE_ECX , uRegValue & 0xFFFF);
				break;
			case 'xd'://dx
				SetContextData(UE_EDX ,  uRegValue & 0xFFFF);
				break;
			case 'xb'://bx
				SetContextData(UE_EBX , uRegValue & 0xFFFF);
				break;
			case 'is'://si
				SetContextData(UE_ESI , uRegValue & 0xFFFF); ;
				break;
			case 'id'://di
				SetContextData(UE_EDI , uRegValue & 0xFFFF);
				break;
			case 'ps'://sp
				SetContextData(UE_ESP , uRegValue & 0xFFFF);
				break;
			case 'pb'://bp
				SetContextData(UE_EBP , uRegValue & 0xFFFF);
				break;
			case 'la'://al
				SetContextData(UE_EAX , uRegValue & 0x0f);
				break;
			case 'ha'://ah
				SetContextData(UE_EAX , uRegValue & 0xf0);
				break;
			case 'lc'://cl
				SetContextData(UE_ECX , uRegValue & 0x0f);
				break;
			case 'hc'://ch
				SetContextData(UE_ECX , uRegValue & 0xf0);
				break;
			case 'ld'://dl
				SetContextData(UE_EDX , uRegValue & 0x0f);
				break;
			case 'hd'://dh
				SetContextData(UE_EDX , uRegValue & 0xf0);
				break;
			case 'lb'://bl
				SetContextData(UE_EBX , uRegValue & 0x0f);
				break;
			case 'hb'://bh
				SetContextData(UE_EBX , uRegValue & 0xf0);
				break;
			default:
				 0;
				return false;
		}
	}


	return true;
}


bool Expression::Value(SSIZE_T& uValue , const char* pStr , const char** pEnd , int nPriorty)
{
	// �жϱ��ʽ�����ڴ��ַ�ı��ʽ������ͨ��ֵ���ʽ
	bool   bFalg = true;
	pStr = skipspace(pStr);

	if(*pStr == 0)
		return 0;


	// �жϱ��ʽ�Ƿ����ڴ�Ѱַ
	// ������ڴ�Ѱַ, ����ڴ�Ѱַ�ı��ʽ��Ϊ�ӱ��ʽ,�ӱ��ʽ��Ҫ������ֵ
	if(*pStr == '[')
	{
		// �õ��ӱ��ʽ��ֵ
		if(!Value(uValue , pStr + 1 , &pStr , 9))
		{
			bFalg = false;
		}
	}
	else if(_strnicmp(pStr , "BYTE PTR" , 8) == 0)
	{
		// �õ��ӱ��ʽ��ֵ
		pStr = skipspace(pStr + 8);
		Value(uValue , pStr , &pStr , 9);
		if(*pStr == ']')	
		{
			++pStr;
			// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
			uValue = readProcMemValue((LPVOID)uValue , sizeof(BYTE));
		}
		else
		{
			bFalg = false;
		}
	}
	else if(_strnicmp(pStr , "WORD PTR" , 8) == 0)
	{
		pStr = skipspace(pStr + 8);
		// �õ��ӱ��ʽ��ֵ
		Value(uValue , pStr , &pStr , 9);
		// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
		if(*pStr == ']')
		{
			++pStr;
			// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
			uValue = readProcMemValue((LPVOID)uValue , sizeof(WORD));
		}
		else
		{
			bFalg = false;
		}
	}
	else if(_strnicmp(pStr , "DWORD PTR" , 9)==0)
	{
		pStr = skipspace(pStr + 9);
		// �õ��ӱ��ʽ��ֵ
		Value(uValue , pStr , &pStr , 9);
		// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
		if(*pStr == ']')
		{
			++pStr;
			// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
			uValue = readProcMemValue((LPVOID)uValue , sizeof(DWORD));
		}
		else
		{
			bFalg = false;
		}
	}
	else if(_strnicmp(pStr , "QWORD" , 5) == 0)
	{
		pStr = skipspace(pStr + 5);
		// �õ��ӱ��ʽ��ֵ
		Value(uValue , pStr , &pStr , 9);
		// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
		if(*pStr == ']')
		{
			++pStr;
			// uValue ���ڴ��ַ,��ȡ���ڴ��ַ����ֵ
			uValue = readProcMemValue((LPVOID)uValue , sizeof(__int64));
		}
		else
		{
			bFalg = false;
		}
	}
	else if(*pStr == '0'&&pStr[ 1 ] == 'x')
	{
		uValue = strtol(pStr , (char**)&pStr , 16);
	}
	else if(isReg(pStr , NULL)) // �����Ǹ��Ĵ�����ֵ
	{
		
		const char *pReg = pStr;
		isReg(pStr , &pReg);
		pReg = skipspace(pReg);
		if(*pReg == '=' && *(pReg+1) != '=') // ���Ĵ�����ֵ
		{
			
			// ��ȡ������
			SSIZE_T nlValue = 0;
			if(Value(nlValue , pReg + 1 , &pReg , 9))
			{
				uValue = nlValue;
				WriteRegValue(pStr , &pStr , nlValue);
				pStr = pReg;
			}
			else
				bFalg = false;
		}
		else
		{
			if(!readRegValue(pStr , &pStr , uValue))
				bFalg = false;
		}
	}
	else if('0' <= *pStr && *pStr <= '9')
	{
		uValue = strtol(pStr , (char**)&pStr , 10);
	}
	else if(*pStr == '+')
	{
		pStr = skipspace(pStr + 1);
		uValue = Value(uValue , pStr , &pStr , 0);
	}
	else if(*pStr == '-')
	{
		pStr = skipspace(pStr + 1);
		if(Value(uValue , pStr , &pStr , 0))
		{
			uValue = -uValue;
		}
	}
	else if(*pStr == '(') /*���ŵ����ȼ����*/
	{
		pStr = skipspace(pStr + 1);
		Value(uValue , pStr , &pStr , 9);
		pStr = skipspace(pStr);
		if(*pStr == ')')
			pStr = skipspace(pStr + 1);
		else
			bFalg = false;
	}
	else
	{
		bFalg = false;
	}



	pStr = skipspace(pStr);

	/* �õ������ */
	if(bFalg == false)
		return false;

	if(*pStr == '+' && nPriorty > 3)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 3))
			return false;
		uValue += right;
	}
	else if(*pStr == '-'  && nPriorty > 3)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 3))
			return false;
		uValue -= right;
	}
	else if(*pStr == '*'  && nPriorty > 2)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 2))
			return false;
		uValue *= right;
	}
	else if(*pStr == '/'  && nPriorty > 2)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 2))
			return false;
		uValue /= right;
	}
	else if(*pStr == '%'  && nPriorty > 2)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 2))
			return false;
		uValue %= right;
	}
	else if(*pStr == '<' && nPriorty>9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 9))
			return false;
		uValue = uValue < right;
	}
	else if(*pStr == '>'&& nPriorty>9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 1 , &pStr , 9))
			return false;
		uValue = uValue > right;
	}
	else if(*pStr == '='&&pStr[ 1 ] == '='&& nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 2 , &pStr , 9))
			return false;
		uValue = uValue == right;
	}
	else if(*pStr == '&'&&pStr[ 1 ] == '&' && nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr +2 , &pStr , 9))
			return false;
		uValue = uValue && right;
	}
	else if(*pStr == '|'&&pStr[ 1 ] == '|'&& nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 2 , &pStr , 9))
			return false;
		uValue = uValue || right;
	}
	else if(*pStr == '<'&&pStr[ 1 ] == '='&& nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 2 , &pStr , 9))
			return false;
		uValue = uValue <= right;
	}
	else if(*pStr == '>'&&pStr[ 1 ] == '='&& nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 2 , &pStr , 9))
			return false;
		uValue = uValue >= right;
	}
	else if(*pStr == '!'&&pStr[ 1 ] == '='&& nPriorty>=9)
	{
		// ��ȡ�ڶ���������
		SSIZE_T right = 0;
		if(!Value(right , pStr + 2 , &pStr , 9))
			return false;
		uValue = uValue != right;
	}


	*pEnd = pStr;
	return bFalg;
}


SSIZE_T Expression::Value(const char* pStr)
{
	if(pStr == nullptr)
		return 0;

	const char* pEnd = pStr;
	SSIZE_T uValue = 0;
	if(Value(uValue , pStr , &pEnd , 9))
		return uValue;
	return 0;
}

