#include "Interface.h"
#include "resource.h"
#include <string.h>
#include <windowsx.h>
#include <commctrl.h>
#pragma comment(lib,"Comctl32.lib")
HWND g_hDlg1;
RECT g_stcClientRt;
char g_szEndAddr[ 32 ];



VOID CALLBACK TimeProc(HWND hWnd , UINT u1, UINT_PTR u2 , DWORD d)
{
	static int nTimes = 5;
	SetLayeredWindowAttributes(hWnd , 0 , nTimes * 25 , LWA_ALPHA);

	if(nTimes++ >= 10)
	{
		KillTimer(hWnd , u2);
		nTimes = 5;
	}

	
}

void  DlgSetStartButtonText(const char* pszText , bool bEnable)
{
	HWND hBtn = GetDlgItem(g_hDlg1 , BTN_START);
	SetWindowTextA(hBtn, pszText);
	EnableWindow(hBtn , bEnable);
}


void DlgSetEndAddress(duint uAddress)
{
	if(g_hDlg1 == NULL)
		return ;

	char szAddress[ 20 ];
	sprintf_s(szAddress , 20 , "%08X" , uAddress);

	SetDlgItemTextA(g_hDlg1 , EDIT_ENDADDR , szAddress);

	SetTimer(g_hDlg1 , 0x1000 , 100 , TimeProc);
}


duint DlgGetEndAddress()
{
	if(g_hDlg1 == NULL)
		return 0;

	char szAddress[ 20 ];

	GetDlgItemTextA(g_hDlg1 , 
					EDIT_ENDADDR , 
					szAddress , 
					sizeof(szAddress));

	return DbgValFromString(szAddress);
}

bool  DlgIsDisableGUI()
{
	if(g_hDlg1 == NULL)
		return false;

	return SendMessage(GetDlgItem(g_hDlg1 , CHEBOX_DISGUI) ,
					   BM_GETCHECK ,
					   0 , 0
					   ) == BST_CHECKED;
}

bool  DlgIsDontTrackSystemDLL()
{
	if(g_hDlg1 == NULL)
		return false;

	return SendMessage(GetDlgItem(g_hDlg1 , CHEBOX_DONTSYS) ,
					   BM_GETCHECK ,
					   0 , 0
					   ) == BST_CHECKED;
}

bool  DlgIsDontTrackOtherDLL()
{
	if(g_hDlg1 == NULL)
		return false;

	return SendMessage(GetDlgItem(g_hDlg1 , CHEBOX_DONTOTHER) ,
					   BM_GETCHECK ,
					   0 , 0
					   ) == BST_CHECKED;
}

bool  DlgIsStepInto()
{
	if(g_hDlg1 == NULL)
		return false;

	return SendMessage(GetDlgItem(g_hDlg1 , RIDO_STEPINTO) ,
					   BM_GETCHECK ,
					   0 , 0
					   ) == BST_CHECKED;
}

bool SaveAs(HWND hParent,char* pszPath)
{
	if(pszPath == NULL)
		return false;

	*pszPath = 0;

	OPENFILENAMEA ofn =
	{
		sizeof(OPENFILENAME) ,
		hParent ,
		NULL ,
		("log file\0*.log\0All Files\0*.*\0\0") ,
		NULL ,
		0 ,
		1 ,
		pszPath , 
		MAX_PATH ,
		NULL , 0 , NULL , 
		"select log file" ,
		OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST |
		OFN_HIDEREADONLY | OFN_NOREADONLYRETURN , 0 , 0 ,
		".log" ,   
		0 ,
		NULL ,
		NULL
	};
	
	return GetSaveFileNameA(&ofn) == TRUE;
}


INT_PTR CALLBACK DlgProc1(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{

	switch(uMsg)
	{
		case WM_INITDIALOG:
		{
			InitCommonControls();
			HWND hSlid = GetDlgItem(hWnd , SLID_ALPHA);
			SendMessage(hSlid , TBM_SETRANGE , 1 , MAKEWPARAM(0 , 255));
			SendMessage(hSlid , TBM_SETPOS , TRUE , 180);

			
			if(g_stcClientRt.left == 0)
			{
				RECT desktopRect;
				RECT clientRect;
				GetClientRect(GetDesktopWindow() , &desktopRect);
				GetClientRect(hWnd , &clientRect);

				SetWindowPos(hWnd ,
							 HWND_TOPMOST ,
							 (desktopRect.right - clientRect.right) / 2 ,
							 (desktopRect.bottom - clientRect.bottom) / 2 ,
							 0 , 0 ,
							 SWP_NOSIZE
							 );
			}
			else
			{

				SetWindowPos(hWnd ,
							 HWND_TOPMOST ,
							 g_stcClientRt.left,
							 g_stcClientRt.top,
							 0 , 0 ,
							 SWP_NOSIZE
							 );
				SetDlgItemTextA(hWnd , EDIT_ENDADDR , g_szEndAddr);
			}
			

			// Set Setpinto Ridio button
			SendMessage(GetDlgItem(hWnd , RIDO_STEPINTO) ,
						BM_SETCHECK ,
						BST_CHECKED ,
						0
						);
						

		
			LONG nRet = ::GetWindowLong(hWnd , GWL_EXSTYLE);
			nRet = nRet | WS_EX_LAYERED;
			::SetWindowLong(hWnd , GWL_EXSTYLE , nRet);
			SetLayeredWindowAttributes(hWnd , 0 , 222 , LWA_ALPHA);
		}
		break;




		case WM_HSCROLL:
		{
			if(LOWORD(wParam) == SB_THUMBTRACK)
			{
				int nPos = SendMessage(GetDlgItem(hWnd , SLID_ALPHA) , TBM_GETPOS , 0 , 0);
				if(nPos <= 50)
				{
					static int nSwitch = 0;
					int nLang[] = { LANG_CHINESE , LANG_ENGLISH };
					int nSubLan[] = { SUBLANG_CHINESE_SIMPLIFIED , SUBLANG_ENGLISH_US };
					SetThreadUILanguage(MAKELANGID(nLang[ nSwitch% _countof(nLang) ] , nSubLan[ nSwitch% _countof(nLang) ]));
					++nSwitch;
					DestroyWindow(hWnd);
					CreateDlg(g_hIns , 0);
					ShowDlg(g_hDlg1 , SW_SHOW);
				}
				else
					SetLayeredWindowAttributes(hWnd , 0 , nPos , LWA_ALPHA);
			}
		}
		break;
		


		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case CHEBOX_DONTOTHER:
				{
					SendMessage(GetDlgItem(hWnd , CHEBOX_DONTSYS) ,
								BM_SETCHECK ,
								(SendMessage(GetDlgItem(hWnd , CHEBOX_DONTOTHER) , BM_GETCHECK , 0 , 0)) ,
								0
								);
				}
				break;

				case BTN_START:
				{
					StopTrack();
					char buff[ 20 ];
					LoadStringA(g_hIns , STR_TRACKING , buff , 20);
					DlgSetStartButtonText(buff , true);

					DlgSetStartButtonText("ÕýÔÚ×·×Ù" , false);
					if(!StartTrack())
						MessageBoxA(hWnd , "Start Track Fail" , "Tips" , 0);
				}
				break;

				case BTN_SAVELOG:
				{
					PauseTrack(true);
					char path[ MAX_PATH ];

					if(!SaveAs(hWnd , path))
						break;

					if(SaveLog(path)){
						if(IDYES == MessageBoxA(hWnd , path , "do you want open it?" , MB_YESNO))
							ShellExecuteA(hWnd , "open" , path , NULL , NULL , NULL);
					}
					else
						MessageBoxA(hWnd , "Save Log Fail" , "Error" , 0);
				}
				break;

			}
		}
		break;




		



		case WM_CLOSE:
			ShowWindow(hWnd , SW_HIDE);
			break;
		case WM_DESTROY:
			GetWindowRect(hWnd , &g_stcClientRt);
			GetDlgItemTextA(hWnd , EDIT_ENDADDR , g_szEndAddr , sizeof(g_szEndAddr));
			break;




		default:
			return FALSE;
	}

	return TRUE;
}

