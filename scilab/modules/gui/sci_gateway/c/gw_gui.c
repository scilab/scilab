/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_gui.h"
#include "stack-c.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/
typedef int (*Gui_Interf) __PARAMS((char *fname,unsigned long l));
typedef struct table_struct 
{
	Gui_Interf f;    /** function **/
	char *name;      /** its name **/
} GuiTable;
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
  {sci_raise_window,"raise_window"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_gui)(void)
{  
	Rhs = Max(0, Rhs);

	if ( getScilabMode() == SCILAB_NWNI)
	{
		Scierror(999,"interface disabled in -nogui mode.");
		return 0;
	}

#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
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
