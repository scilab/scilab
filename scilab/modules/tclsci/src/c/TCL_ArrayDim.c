/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "TCL_ArrayDim.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
#define TCL_SCILAB_VARIABLE_TMP "TclScilabTmpVar"
#define CHAR_BLANK ' '
#define CHAR_PERIOD ','
/*--------------------------------------------------------------------------*/
BOOL TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n)
{
	BOOL Bok = FALSE;
	*m=0;
	*n=0;
	if (strcmp(VarName,TCL_SCILAB_VARIABLE_TMP))
		{
		char MyTclCommand[2048];
		char *StrArrayDims=NULL;

		sprintf(MyTclCommand, "set TclScilabTmpVar [lsort -dictionary [array names %s *,*]];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,_("Tcl Error : %s\n"),TCLinterpreter->result);
			return 0;
		}

		StrArrayDims = (char *) Tcl_GetVar(TCLinterpreter, TCL_SCILAB_VARIABLE_TMP,TCL_GLOBAL_ONLY);

		if (StrArrayDims)
		{
			char *token = NULL;
			char StrDimensions[256];
			int DimX;
			int DimY;
			token = strrchr( StrArrayDims,CHAR_BLANK);
			if (token)
			{
				int loop=0;
				if (token[0]==CHAR_BLANK) token=&token[1];
				for(loop=0;loop<(int)strlen(token);loop++) 
					{
						if (token[loop] == CHAR_PERIOD) 
							{
								token[loop]=CHAR_BLANK;
							}
					}
				strcpy(StrDimensions,token);
				sscanf(StrDimensions,"%d %d",&DimX,&DimY);
				*n=DimX;
				*m=DimY;
			}
			else /* just a element */
			{
				int loop=0;
				for(loop=0;loop<(int)strlen(StrArrayDims);loop++) if ( StrArrayDims[loop] == CHAR_PERIOD)  StrArrayDims[loop]=CHAR_BLANK;
				strcpy(StrDimensions,StrArrayDims);
				sscanf(StrDimensions,"%d %d",&DimX,&DimY);
				*n=DimX;
				*m=DimY;
			}
			Tcl_UnsetVar(TCLinterpreter, TCL_SCILAB_VARIABLE_TMP, TCL_GLOBAL_ONLY);
			Bok = TRUE;
		}
	}
	return Bok;
}
/*--------------------------------------------------------------------------*/
