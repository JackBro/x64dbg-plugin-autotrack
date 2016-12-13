#include "Interface.h"
#include "pluginmain.h"
#include "resource.h"
using namespace Script;

HINSTANCE g_hIns;
int pluginHandle;
HWND hwndDlg;
int hMenu;
int hMenuDisasm;
int hMenuDump;
int hMenuStack;


BOOL WINAPI DllMain(_In_ HANDLE _HDllHandle , _In_ DWORD _Reason , _In_opt_ LPVOID _Reserved)
{
	g_hIns = (HINSTANCE)_HDllHandle;
	return TRUE;
}



// debuger event 
void EventCallBack(CBTYPE cbType , void* callbackinfo)
{
	switch(cbType)
	{
		case CB_STEPPED:
			CB_StepEventCallBack(cbType , callbackinfo);
		break;

		case CB_BREAKPOINT:
			CB_BreakPointEventCallBack(cbType , callbackinfo);
		break;
	
	}
}


PLUG_EXPORT void CBMENUENTRY(CBTYPE cbType , PLUG_CB_MENUENTRY* info)
{
	switch(info->hEntry)
	{
		// right click at disasmble window
		case MENU_DISASM_SETENDADDR: 
		{
			// get cursor address 
			// Send It to Main Window
			DlgSetEndAddress(Script::Gui::Disassembly::SelectionGetStart());
		}
		// click at main menu 
		case MENU_AUTOTRACK: 
		{
			// Show Main Dialog
			ShowDlg(g_hDlg1,SW_SHOW);
		}
		break;

		case MENU_DISASM_SETCONDITIONBREAKPOINT: // condition breakpoint
		{
			DlgSetBreakpintAddress(Script::Gui::Disassembly::SelectionGetStart());
			ShowDlg(g_hDlg2 , SW_SHOW);
		}
		break;
	}
}






PLUG_EXPORT bool pluginit(PLUG_INITSTRUCT* initStruct)
{
	initStruct->pluginVersion = PLUGIN_VERSION;
	initStruct->sdkVersion = PLUG_SDKVERSION;
	strncpy_s(initStruct->pluginName , PLUGIN_NAME , _TRUNCATE);
	pluginHandle = initStruct->pluginHandle;
	

	// Create Dialog, And Hide it.
	CreateDlg(g_hIns , 0);
	CreateDlg(g_hIns , 1);
	ShowWindow(g_hDlg1 , SW_HIDE);
	ShowWindow(g_hDlg2 , SW_HIDE);

	AddSystemDllName();

	return true;

}




PLUG_EXPORT void plugsetup(PLUG_SETUPSTRUCT* setupStruct)
{
	hwndDlg = setupStruct->hwndDlg;
	hMenu = setupStruct->hMenu;
	hMenuDisasm = setupStruct->hMenuDisasm;
	hMenuDump = setupStruct->hMenuDump;
	hMenuStack = setupStruct->hMenuStack;
	

	/*add menu*/
	_plugin_menuaddentry(hMenu , MENU_AUTOTRACK , "&Auto Track");
	_plugin_menuaddentry(hMenuDisasm , MENU_DISASM_SETENDADDR , "&Set address as track endAddress");
	_plugin_menuaddentry(hMenuDisasm , MENU_DISASM_SETCONDITIONBREAKPOINT , "&Set condition breakpoint");

	_plugin_registercallback(pluginHandle , CB_STEPPED , EventCallBack);
	_plugin_registercallback(pluginHandle , CB_BREAKPOINT , EventCallBack);
	
}


PLUG_EXPORT bool plugstop()
{
	// register command callback function
	_plugin_unregistercommand(pluginHandle , PLUGIN_NAME);

	// clear all menu
	_plugin_menuclear(hMenu);
	_plugin_menuclear(hMenuDisasm);
	_plugin_menuclear(hMenuDump);
	_plugin_menuclear(hMenuStack);
	return true;
}