#include "Interface.h"


HWND g_hDlg2;


INT_PTR CALLBACK DlgProc2(HWND hWnd , UINT uMsg , WPARAM wParam , LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
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

			// Set Setpinto Ridio button
			SendMessage(GetDlgItem(hWnd , RIDO_STEPINTO) ,
						BM_SETCHECK ,
						BST_CHECKED ,
						0
						);
		}
		break;

		case WM_COMMAND:
		{
			if(LOWORD(wParam) == BTN_OK)
			{
				char	buff[ 512 ];
				GetDlgItemTextA(hWnd , EDIT_BPADDR , buff , sizeof(buff));

				duint uAddress = DbgValFromString(buff);

				GetDlgItemTextA(hWnd , EDIT_BPCONDITION , buff , sizeof(buff));
				if(!AddBreakpoint(uAddress , buff))
					MessageBoxA(hWnd , "Add Breakpoint Fail" , "Error" , 0);
			}
		}
		break;

		case WM_CLOSE:
			ShowWindow(hWnd , SW_HIDE);
			break;

		default:
			return FALSE;
	}

	return TRUE;
}

duint DlgGetBreakpointAddress()
{
	char	buff[ 512 ];
	GetDlgItemTextA(g_hDlg2 , EDIT_BPADDR , buff , sizeof(buff));
	return DbgValFromString(buff);
}


void  DlgGetBreakpointConditon(char* condition , int nMaxSize)
{
	GetDlgItemTextA(g_hDlg2 , EDIT_BPCONDITION , condition , nMaxSize);
}



void  DlgSetBreakpintAddress(duint uAddress)
{
	char buff[ 32 ];
	sprintf_s(buff ,32, "%x" , uAddress);
	SetDlgItemTextA(g_hDlg2 , EDIT_BPADDR , buff);
}



void  DlgSetConditon(const char* pszCondition)
{
	if(pszCondition == nullptr)
		return;

	SetDlgItemTextA(g_hDlg2 , EDIT_BPCONDITION , pszCondition);
}