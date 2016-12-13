#pragma once
#include "pluginmain.h"
#include "resource.h"

extern HINSTANCE g_hIns;
extern HWND g_hDlg1;
extern HWND g_hDlg2;
INT_PTR CALLBACK DlgProc1(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);
INT_PTR CALLBACK DlgProc2(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam);





























//////////////////////////////////////////////////////////////////////////
// 
inline void CreateDlg(HINSTANCE hInstance , DWORD dwCode)
{
	if(dwCode == 0)
	{
		g_hDlg1 = CreateDialog(hInstance ,
							   MAKEINTRESOURCE(IDD_DIALOG1) ,
							   0 ,
							   DlgProc1);
	}
	else if(dwCode == 1)
	{
		g_hDlg2 = CreateDialog(hInstance ,
							   MAKEINTRESOURCE(IDD_DIALOG2) ,
							   0 ,
							   DlgProc2);
	}
}


inline void ShowDlg(HWND hDlg , DWORD dwShowMask)
{
	ShowWindow(hDlg , dwShowMask);
}




















//////////////////////////////////////////////////////////////////////////
// AutoTrack Dialog function
void  DlgSetStartButtonText(const char* pszText, bool bEnable= true);

void  DlgSetEndAddress(duint uAddress);
duint DlgGetEndAddress();

bool  DlgIsDisableGUI();
bool  DlgIsDontTrackSystemDLL();
bool  DlgIsDontTrackOtherDLL();
bool  DlgIsStepInto();
















//////////////////////////////////////////////////////////////////////////
// Condition breakpoint Dialog Function

duint DlgGetBreakpointAddress();
void  DlgGetBreakpointConditon(char* condition, int nMaxSize);
void  DlgSetBreakpintAddress(duint uAddress);
void  DlgSetConditon(const char* pszCondition);





























//////////////////////////////////////////////////////////////////////////
// AutoTrack core function

bool  StartTrack();
bool  PauseTrack(bool IsPause);
bool  StopTrack();
bool  SaveLog(const char* pszLogFilePath);
void  CB_StepEventCallBack(CBTYPE cbType , void* callbackinfo);























//////////////////////////////////////////////////////////////////////////
// condition core function

bool  AddBreakpoint(duint , const char* pszCondition);
void  CB_BreakPointEventCallBack(CBTYPE cbType , void* callbackinfo);























//////////////////////////////////////////////////////////////////////////
void AddSystemDllName();
bool IsValidSystemDll(const char* pszModuleName);

