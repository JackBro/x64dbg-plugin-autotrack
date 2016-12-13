#include "Interface.h"
#include <list>
using std::list;
#include <atlstr.h>
#include "pluginsdk\_scriptapi_module.h"
#include "Expression\Expression.h"
#include "resource.h"

// 0 : Stop , 1 : pause , 2 : Start
int  g_nTrackStatus ;


list<CStringA> g_listLog;
typedef list<CStringA>::iterator LOGITERATOR;
Script::Module::ModuleInfo g_stcCurModuleInfo;


duint		g_uEndAddress ;
CStringA	g_strCondition;
bool		g_bIsDisableGUI = false;
bool		g_bIsStepInto = true;
bool		g_bIsDontTrackOtherDll = false;
bool		g_bIsDontTrackSystemDll = false;
char*		g_pStepCmd;


bool  StartTrack()
{
	if(!DbgIsDebugging())
		return false;

	g_listLog.clear();
	g_nTrackStatus = 2;

	g_uEndAddress = DlgGetEndAddress();

	if(g_bIsDisableGUI = DlgIsDisableGUI())
		GuiUpdateDisable();
	else
		GuiUpdateEnable(true);


	g_bIsStepInto = DlgIsStepInto();
	g_pStepCmd = g_bIsStepInto ? "StepInto" : "StepOut";

	g_bIsDontTrackOtherDll = DlgIsDontTrackOtherDLL();
	g_bIsDontTrackSystemDll = DlgIsDontTrackSystemDLL();

	
	Script::Module::InfoFromAddr(Script::Register::GetCIP() ,
								 &g_stcCurModuleInfo);


	Cmd(g_pStepCmd);
	return true;
}

bool  PauseTrack(bool IsPause)
{
	g_nTrackStatus = 1;
	if(IsPause)
		Cmd("pause");
	else
		StartTrack();

	return true;
}


bool  StopTrack()
{
	g_nTrackStatus = 0;
	Cmd("pause");
	return true;
}


bool  SaveLog(const char* pszLogFilePath)
{
	if(pszLogFilePath == nullptr)
		return false;

	if(g_listLog.empty())
		return false;

	FILE* pFile = NULL;
	fopen_s(&pFile , pszLogFilePath , "w+b");
	if(pFile == NULL)
		return false;


	char buff[ 100 ];
	char szString[ 5 ][ 60 ];
	for(int i = 0; i <  5; ++i)
		LoadStringA(g_hIns , i+STR_ADDRESS , szString[ i ] , 60);


	sprintf_s(buff , 100 , "%08X |%6s| %-40s | %8s, %8s\n" ,
			  szString[ 0 ] ,
			  szString[ 1 ] ,
			  szString[ 2 ] ,
			  szString[ 3 ] ,
			  szString[ 4 ] ,
			  szString[ 5 ]
			  );

	fwrite(buff , 1 , strlen(buff) , pFile);
	fwrite("---------+------+------------------------------------------+-------------------------\n" ,
		   1 , 86 , pFile);

	for(auto &i : g_listLog)
		fwrite(i , sizeof(char) , i.GetLength() , pFile);


	fclose(pFile);
	return true;
}




bool GetOperand(const CStringA& str , list<CStringA>& listOperand)
{
	int nFirstOperand = str.Find(' ');
	if(nFirstOperand == -1)
		return false;
	
	int nStart = 0;
	int nLastStart = nFirstOperand;
	while(-1 != (nStart = str.Find(',' , nLastStart)))
	{
		if(nLastStart != nStart)
		{
			listOperand.push_back(str.Mid(nLastStart , nStart - nLastStart));
		}

		nStart++;
		nLastStart = nStart;
	}

	listOperand.push_back(str.Mid(nLastStart , nLastStart));
	return true;
}



void  CB_StepEventCallBack(CBTYPE cbType , void* callbackinfo)
{
	if(cbType != CB_STEPPED)
		return;

	if(g_nTrackStatus != 2)
		return;

	duint rip = Script::Register::GetCIP();

	static Script::Module::ModuleInfo LastModuleInfo;
	Expression				expr;
	CStringA				buff;
	BASIC_INSTRUCTION_INFO	insInfo ;
	duint					uLeftData = 0;
	duint					uRightData = 0;
	CStringA				leftOper;
	CStringA				rightOper;
	list<CStringA>			listOperand;

	const char*				pLeftOperand = "";
	const char*				pRightOperand = "";
	const char*				pStatus = "--  ";

	if(rip == g_uEndAddress)
	{
		buff.GetBufferSetLength(20);
		LoadStringA(g_hIns , STR_STARTTRACK , (char*)(LPCSTR)buff , 20);
		DlgSetStartButtonText(buff , true);

		MessageBoxA(g_hDlg1 , "Finish!" , "Tips" , 0);
		GuiUpdateEnable(true);
		GuiUpdateAllViews();

		Cmd("pause");
		return;
	}

	
	if(!g_strCondition.IsEmpty())
	{
		if(!expr.Value(g_strCondition))
			return;

		goto _KEEPGOING;
	}

	if(rip > LastModuleInfo.base + LastModuleInfo.size)
	{
		Script::Module::InfoFromAddr(rip , &LastModuleInfo);
		buff.Format("						      < ModuleName:%s >\n" , LastModuleInfo.name);
		g_listLog.push_back(buff);
	}

	if(g_bIsDontTrackOtherDll)
	{
		if(rip > g_stcCurModuleInfo.base + g_stcCurModuleInfo.size)
			goto _KEEPGOING ;
	}
	else if(g_bIsDontTrackSystemDll) 
	{
		Script::Module::ModuleInfo modInfo;
		Script::Module::InfoFromAddr(rip , &modInfo);

		if(IsValidSystemDll(modInfo.name))
			goto _KEEPGOING ;
	}



	DbgDisasmFastAt(rip , &insInfo);
	if(insInfo.branch && !insInfo.call)
		pStatus = DbgIsJumpGoingToExecute(rip) ? "True " : "Flase";

	// Get operands
	GetOperand(insInfo.instruction , listOperand);
	if(listOperand.size() >= 1){
		pLeftOperand = *listOperand.begin();
		// get value of operaand 
		uLeftData = expr.Value(pLeftOperand);
	}
	if(listOperand.size() >= 2){
		list<CStringA>::iterator itr = listOperand.begin();
		++itr;
		pRightOperand = *itr;
		// get value of operaand 
		uRightData = expr.Value(pRightOperand);
	}


	
	buff.Format("%08X |%6s| %-40s | L: <%s=%X>, R: <%s=%X>\n" ,
				rip ,
				pStatus ,
				insInfo.instruction ,
				pLeftOperand ,
				uLeftData ,
				pRightOperand ,
				uRightData
				);


	g_listLog.push_back(buff);


_KEEPGOING:
	Cmd(g_pStepCmd);

}
