/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "TCL_ArrayDim.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
#define TCL_SCILAB_VARIABLE_TMP "TclScilabTmpVar"
#define CHAR_BLANK ' '
#define CHAR_PERIOD ','
/*-----------------------------------------------------------------------------------*/
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
			error_scilab(999,"Tcl Error : %s",TCLinterpreter->result);
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
				for(loop=0;loop<(int)strlen(token);loop++) if (token[loop] == CHAR_PERIOD) token[loop]=CHAR_BLANK;
				sprintf(StrDimensions,"%s",token);
				sscanf(StrDimensions,"%d %d",&DimX,&DimY);
				*n=DimX;
				*m=DimY;
			}
			else /* just a element */
			{
				int loop=0;
				for(loop=0;loop<(int)strlen(StrArrayDims);loop++) if ( StrArrayDims[loop] == CHAR_PERIOD)  StrArrayDims[loop]=CHAR_BLANK;
				sprintf(StrDimensions,"%s", StrArrayDims);
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
/*-----------------------------------------------------------------------------------*/
