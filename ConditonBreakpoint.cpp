#include "Interface.h"
#include <map>
using std::map;
using std::pair;

#include <atlstr.h>
#include "Expression/Expression.h"

map<duint ,CStringA> g_mapBp;

bool  AddBreakpoint(duint uAddr, const char* pszCondition)
{
	if(!Script::Debug::SetBreakpoint(uAddr))
		return false;
	
	pair<map<duint , CStringA>::iterator,bool> itr = g_mapBp.insert(pair<duint , CStringA>(uAddr , pszCondition));
	if(itr.second == false)
		itr.first->second = pszCondition;
	

	return true;
}


void  CB_BreakPointEventCallBack(CBTYPE cbType , void* callbackinfo)
{
	if(cbType != CB_BREAKPOINT)
		return;
	if(g_mapBp.empty())
		return;

	duint uIP = Script::Register::GetCIP();
	Expression expr;
	
	map<duint , CStringA>::iterator itr = g_mapBp.find(uIP);
	if(itr == g_mapBp.end())
		return;

	if(expr.Value((*itr).second) == 0)
		Cmd("go");

}