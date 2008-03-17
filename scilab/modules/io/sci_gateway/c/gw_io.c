/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "gw_io.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_oldload),"oldload"},
{C2F(sci_read),"read"},
{C2F(sci_getf),"getf"},
{C2F(sci_exec),"exec"},
{C2F(sci_lib),"lib"},
{C2F(sci_diary),"diary"},
{C2F(sci_oldsave),"oldsave"},
{C2F(sci_write),"write"},
{C2F(sci_print),"print"},
{C2F(sci_rat),"rat"},
{C2F(sci_deff),"deff"},
{C2F(sci_file),"file"},
{C2F(sci_host),"host"},
{C2F(sci_unix),"unix"},
{C2F(sci_readb),"readb"},
{C2F(sci_writb),"writb"},
{C2F(sci_execstr),"execstr"},
{C2F(sci_disp),"disp"},
{C2F(sci_getpid),"getpid"},
{C2F(sci_getenv),"getenv"},
{C2F(sci_read4b),"read4b"},
{C2F(sci_write4b),"write4b"},
{C2F(sci_save),"save"},
{C2F(sci_load),"load"},
{C2F(sci_mgetl),"mgetl"},
{C2F(sci_fileinfo),"fileinfo"},
{C2F(sci_getio),"getio"},
{C2F(sci_setenv),"setenv"},
{C2F(sci_getrelativefilename),"getrelativefilename"},
{C2F(sci_newest),"newest"},
{C2F(sci_get_absolute_file_path),"get_absolute_file_path"}
};
/*--------------------------------------------------------------------------*/
int gw_io(void)
{  
	 /* Recursion */
	 if (C2F(recu).rstk[C2F(recu).pt-1] / 100 == 9)
	 {
		switch ((int)(C2F(recu).rstk[C2F(recu).pt-1] - 901))
		{
			case 1: 
			{
				C2F(intexec)("exec",(unsigned long)strlen("exec"));
				return 0;
			}
			case 2: 
			{
				C2F(intexecstr)("execstr",(unsigned long)strlen("execstr"));
				return 0;
			}
			case 3: 
			{
				C2F(intgetf)(); 
				return 0;
			}
			case 4:  
			{
				C2F(intsave)(); 
				return 0;
			}
			case 5:
			{
				C2F(sci_load)("load",(unsigned long)strlen("load"));
			}
			case 6: 
			{
				return 0;
			}
			case 7:
			{
				C2F(intdisp)();
				return 0;
			}
			case 8: 
			{
				C2F(intexec)("exec",(unsigned long)strlen("exec"));
				return 0;
			}
		}
	}
	 else
	 {
		Rhs = Max(0, Rhs);
		callFunctionFromGateway(Tab);
	 }
	return 0;
}
/*--------------------------------------------------------------------------*/
