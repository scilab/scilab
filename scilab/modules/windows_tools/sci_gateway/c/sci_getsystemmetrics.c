/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int getnIndexFromString(char *nIndex_char);
/*--------------------------------------------------------------------------*/
int sci_getsystemmetrics(char *fname,unsigned long l)
{
	char *param = NULL;
	int nIndex = -1;
	int m1 = 0;
	int n1 = 0;
	int l1 = 0;

	CheckRhs(1,1);

	if (GetType(1) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
		return 0;
	}
	
	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	param = cstk(l1);

	nIndex = getnIndexFromString(param);
	if ( nIndex > -1 )
	{
		int one = 1;
		CreateVar(Rhs+1,MATRIX_OF_INTEGER_DATATYPE,&one,&one,&l1);
		*istk(l1) = GetSystemMetrics(nIndex);
		LhsVar(1) = Rhs+1;

		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument: %s.\n"),fname,_("see help"));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int getnIndexFromString(char *nIndex_char)
{
if (_stricmp("SM_ARRANGE",nIndex_char) == 0)                       return SM_ARRANGE;
if (_stricmp("SM_CLEANBOOT",nIndex_char) == 0)                     return SM_CLEANBOOT;
if (_stricmp("SM_CMONITORS",nIndex_char) == 0)                     return SM_CMONITORS;
if (_stricmp("SM_CMOUSEBUTTONS",nIndex_char) == 0)                 return SM_CMOUSEBUTTONS;
if (_stricmp("SM_CXBORDER",nIndex_char) == 0)                      return SM_CXBORDER;
if (_stricmp("SM_CXCURSOR",nIndex_char) == 0)                      return SM_CXCURSOR;
if (_stricmp("SM_CXDLGFRAME",nIndex_char) == 0)                    return SM_CXDLGFRAME;
if (_stricmp("SM_CXDOUBLECLK",nIndex_char) == 0)                   return SM_CXDOUBLECLK;
if (_stricmp("SM_CXDRAG",nIndex_char) == 0)                        return SM_CXDRAG;
if (_stricmp("SM_CXEDGE",nIndex_char) == 0)                        return SM_CXEDGE;
if (_stricmp("SM_CXFIXEDFRAME",nIndex_char) == 0)                  return SM_CXFIXEDFRAME;
if (_stricmp("SM_CXFOCUSBORDER",nIndex_char) == 0)                 return SM_CXFOCUSBORDER;
if (_stricmp("SM_CXFRAME",nIndex_char) == 0)                       return SM_CXFRAME;
if (_stricmp("SM_CXFULLSCREEN",nIndex_char) == 0)                  return SM_CXFULLSCREEN;
if (_stricmp("SM_CXHSCROLL",nIndex_char) == 0)                     return SM_CXHSCROLL;
if (_stricmp("SM_CXHTHUMB",nIndex_char) == 0)                      return SM_CXHTHUMB;
if (_stricmp("SM_CYICON",nIndex_char) == 0)                        return SM_CYICON;
if (_stricmp("SM_CYICONSPACING",nIndex_char) == 0)                 return SM_CYICONSPACING;
if (_stricmp("SM_CYKANJIWINDOW",nIndex_char) == 0)                 return SM_CYKANJIWINDOW;
if (_stricmp("SM_CYMAXIMIZED",nIndex_char) == 0)                   return SM_CYMAXIMIZED;
if (_stricmp("SM_CYMAXTRACK",nIndex_char) == 0)                    return SM_CYMAXTRACK;
if (_stricmp("SM_CYMENU",nIndex_char) == 0)                        return SM_CYMENU;
if (_stricmp("SM_CYMENUCHECK",nIndex_char) == 0)                   return SM_CYMENUCHECK;
if (_stricmp("SM_CYMENUSIZE",nIndex_char) == 0)                    return SM_CYMENUSIZE;
if (_stricmp("SM_CYMIN",nIndex_char) == 0)                         return SM_CYMIN;
if (_stricmp("SM_CYMINIMIZED",nIndex_char) == 0)                   return SM_CYMINIMIZED;
if (_stricmp("SM_CYMINSPACING",nIndex_char) == 0)                  return SM_CYMINSPACING;
if (_stricmp("SM_CYMINTRACK",nIndex_char) == 0)                    return SM_CYMINTRACK;
if (_stricmp("SM_CYSCREEN",nIndex_char) == 0)                      return SM_CYSCREEN;
if (_stricmp("SM_CYSIZE",nIndex_char) == 0)                        return SM_CYSIZE;
if (_stricmp("SM_CYSIZEFRAME",nIndex_char) == 0)                   return SM_CYSIZEFRAME;
if (_stricmp("SM_CYSMCAPTION",nIndex_char) == 0)                   return SM_CYSMCAPTION;
if (_stricmp("SM_CYSMICON",nIndex_char) == 0)                      return SM_CYSMICON;
if (_stricmp("SM_CYSMSIZE",nIndex_char) == 0)                      return SM_CYSMSIZE;
if (_stricmp("SM_CYVIRTUALSCREEN",nIndex_char) == 0)               return SM_CYVIRTUALSCREEN;
if (_stricmp("SM_CYVSCROLL",nIndex_char) == 0)                     return SM_CYVSCROLL;
if (_stricmp("SM_CYVTHUMB",nIndex_char) == 0)                      return SM_CYVTHUMB;
if (_stricmp("SM_DBCSENABLED",nIndex_char) == 0)                   return SM_DBCSENABLED;
if (_stricmp("SM_DEBUG",nIndex_char) == 0)                         return SM_DEBUG;
if (_stricmp("SM_IMMENABLED",nIndex_char) == 0)                    return SM_IMMENABLED;
if (_stricmp("SM_MEDIACENTER",nIndex_char) == 0)                   return SM_MEDIACENTER;
if (_stricmp("SM_MENUDROPALIGNMENT",nIndex_char) == 0)             return SM_MENUDROPALIGNMENT;
if (_stricmp("SM_MIDEASTENABLED",nIndex_char) == 0)                return SM_MIDEASTENABLED;
if (_stricmp("SM_MOUSEPRESENT",nIndex_char) == 0)                  return SM_MOUSEPRESENT;
if (_stricmp("SM_MOUSEHORIZONTALWHEELPRESENT",nIndex_char) == 0)   return SM_MOUSEHORIZONTALWHEELPRESENT;
if (_stricmp("SM_MOUSEWHEELPRESENT",nIndex_char) == 0)             return SM_MOUSEWHEELPRESENT;
if (_stricmp("SM_NETWORK",nIndex_char) == 0)                       return SM_NETWORK;
if (_stricmp("SM_PENWINDOWS",nIndex_char) == 0)                    return SM_PENWINDOWS;
if (_stricmp("SM_REMOTECONTROL",nIndex_char) == 0)                 return SM_REMOTECONTROL;
if (_stricmp("SM_REMOTESESSION",nIndex_char) == 0)                 return SM_REMOTESESSION;
if (_stricmp("SM_SAMEDISPLAYFORMAT",nIndex_char) == 0)             return SM_SAMEDISPLAYFORMAT;
if (_stricmp("SM_SECURE",nIndex_char) == 0)                        return SM_SECURE;
if (_stricmp("SM_SERVERR2",nIndex_char) == 0)                      return SM_SERVERR2;
if (_stricmp("SM_SHOWSOUNDS",nIndex_char) == 0)                    return SM_SHOWSOUNDS;
if (_stricmp("SM_SHUTTINGDOWN",nIndex_char) == 0)                  return SM_SHUTTINGDOWN;
if (_stricmp("SM_SLOWMACHINE",nIndex_char) == 0)                   return SM_SLOWMACHINE;
if (_stricmp("SM_STARTER",nIndex_char) == 0)                       return SM_STARTER;
if (_stricmp("SM_SWAPBUTTON",nIndex_char) == 0)                    return SM_SWAPBUTTON;
if (_stricmp("SM_TABLETPC",nIndex_char) == 0)                      return SM_TABLETPC;
if (_stricmp("SM_XVIRTUALSCREEN",nIndex_char) == 0)                return SM_XVIRTUALSCREEN;
if (_stricmp("SM_YVIRTUALSCREEN",nIndex_char) == 0)                return SM_YVIRTUALSCREEN;
	return -1;
}
/*--------------------------------------------------------------------------*/
