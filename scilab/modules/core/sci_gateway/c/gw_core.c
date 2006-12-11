#include "gw_core.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_debug) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_who) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_lines) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_argn) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_comp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_mode) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_type) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_error) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_resume) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_return) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_format) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_isdef) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_exists) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_errcatch) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_errclear) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_iserror) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_predef) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_newfun) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_clearfun) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_funptr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_macr2lst) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_setbpt) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_delbpt) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_dispbpt) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_funcprot) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_whereis) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_where) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_havewindow) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_stacksize) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_mtlb_mode) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_link) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ulink) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_c_link) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_addinter) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_clear) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_what) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_sciargs) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_chdir) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getcwd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_pwd) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_ieee) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_typename) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_global) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_clearglobal) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_isglobal) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_gstacksize) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_intppty) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_lasterror) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getversion) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_loadhistory) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_savehistory) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_gethistory) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_resethistory) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_macr2tree) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getos) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_banner) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fromjava) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getmemory) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_fromc) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getmd5) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_warning) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_getmodules) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
static CoreTable Tab[]=
{
{C2F(sci_debug),"debug"},
{C2F(sci_who),"who"},
{C2F(sci_lines),"lines"},
{C2F(sci_argn),"argn"},
{C2F(sci_fort),"fort"},
{C2F(sci_comp),"comp"},
{C2F(sci_call),"call"},
{C2F(sci_mode),"mode"},
{C2F(sci_type),"type"},
{C2F(sci_error),"error"},
{C2F(sci_resume),"resume"},
{C2F(sci_return),"return"},
{C2F(sci_format),"format"},
{C2F(sci_isdef),"isdef"},
{C2F(sci_exists),"exists"},
{C2F(sci_errcatch),"errcatch"},
{C2F(sci_errclear),"errclear"},
{C2F(sci_iserror),"iserror"},
{C2F(sci_predef),"predef"},
{C2F(sci_newfun),"newfun"},
{C2F(sci_clearfun),"clearfun"},
{C2F(sci_funptr),"funptr"},
{C2F(sci_macr2lst),"macr2lst"},
{C2F(sci_setbpt),"setbpt"},
{C2F(sci_delbpt),"delbpt"},
{C2F(sci_dispbpt),"dispbpt"},
{C2F(sci_funcprot),"funcprot"},
{C2F(sci_whereis),"whereis"},
{C2F(sci_where),"where"},
{C2F(sci_havewindow),"havewindow"},
{C2F(sci_stacksize),"stacksize"},
{C2F(sci_mtlb_mode),"mtlb_mode"},
{C2F(sci_link),"link"},
{C2F(sci_ulink),"ulink"},
{C2F(sci_c_link),"c_link"},
{C2F(sci_addinter),"addinter"},
{C2F(sci_clear),"clear"},
{C2F(sci_what),"what"},
{C2F(sci_sciargs),"sciargs"},
{C2F(sci_chdir),"chdir"},
{C2F(sci_ieee),"ieee"},
{C2F(sci_typename),"typename"},
{C2F(sci_global),"global"},
{C2F(sci_pwd),"pwd"},
{C2F(sci_clearglobal),"clearglobal"},
{C2F(sci_isglobal),"isglobal"},
{C2F(sci_gstacksize),"gstacksize"},
{C2F(sci_intppty),"intppty"},
{C2F(sci_lasterror),"lasterror"},
{C2F(sci_getversion),"getversion"},
{C2F(sci_loadhistory),"loadhistory"},
{C2F(sci_savehistory),"savehistory"},
{C2F(sci_gethistory),"gethistory"},
{C2F(sci_resethistory),"resethistory"},
{C2F(sci_macr2tree),"macr2tree"},
{C2F(sci_getos),"getos"},
{C2F(sci_banner),"banner"},
{C2F(sci_fromjava),"fromjava"},
{C2F(sci_getmemory),"getmemory"},
{C2F(sci_fromc),"fromc"},
{C2F(sci_getmd5),"getmd5"},
{C2F(sci_warning),"warning"},
{C2F(sci_getcwd),"getcwd"},
{C2F(sci_getmodules),"getmodules"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_core)()
{  
	Rhs = Max(0, Rhs);

	/**
	* recursion depuis intdeff
	*/
	if ( C2F(recu).rstk[C2F(recu).pt-1] == 901) 
	{
		Fin=6;
	}

#if _MSC_VER
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
