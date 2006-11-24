#include "gw_gui.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "MALLOC.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif

/*-----------------------------------------------------------------------------------*/
static GuiTable Tab[]=
{
	{C2F(sci_x_dialog),"x_dialog"},
	{C2F(sci_x_message),"x_message"},
	{C2F(sci_x_choose),"x_choose"},
	{C2F(sci_x_mdialog),"x_mdialog"},
	{C2F(sci_xchoicesi),"xchoicesi"},
	{C2F(sci_addmenu_old),"addmenu_old"},
	{C2F(sci_delmenu_old),"delmenu_old"},
	{C2F(sci_setmenu_old),"setmenu_old"},
	{C2F(sci_unsetmenu_old),"unsetmenu_old"},
	{C2F(sci_xgetfile),"xgetfile"},
	{C2F(sci_x_message_modeless),"x_message_modeless"},
        {sci_clc,"clc"},
        {sci_show_window,"show_window"},
        {sci_tohome,"tohome"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_gui)(void)
{  
	Rhs = Max(0, Rhs);

	if (! GetWITH_GUI() )
	{
		Scierror(999,"interface disabled in -nogui mode.");
		return 0;
	}

#if _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			char *ExceptionString=GetExceptionString(GetExceptionCode());
			sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
			if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
