/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <string.h>
#ifdef _MSC_VER
#include <Windows.h>
#include "ExceptionMessage.h"
#endif
#include "gw_renderer.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
typedef int (*Renderer_Interf) (char *fname,unsigned long l);

typedef struct table_struct
{
  Renderer_Interf f;    /** function **/
  char *name;      /** its name **/
} RendererTable;
/*--------------------------------------------------------------------------*/
static RendererTable Tab[]=
{
//{C2F(sci_skeleton_c),"skeleton_c"},
};
/*--------------------------------------------------------------------------*/
int gw_renderer(void)
{  
	Rhs = Max(0, Rhs);

#ifdef _MSC_VER
	#ifndef _DEBUG
		_try
		{
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
		}
		_except (EXCEPTION_EXECUTE_HANDLER)
		{	
			ExceptionMessage(GetExceptionCode(),Tab[Fin-1].name);
		}
	#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
	#endif
#else
		(*(Tab[Fin-1].f)) (Tab[Fin-1].name,(unsigned long)strlen(Tab[Fin-1].name));
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
