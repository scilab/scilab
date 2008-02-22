/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "../../localization/includes/localization.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_GetVersion(char *fname,unsigned long l)
{
	static int l1,n1,m1;
	int major=0;
	int minor=0;
	int patchLevel=0;
	int type=0;
	char *output=NULL ;
	char VersionString[256];
	char ReleaseType[256];

	CheckRhs(0,1);
	CheckLhs(1,1);

	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	if (Rhs == 0)
	{
		switch (type)
		{
			case TCL_ALPHA_RELEASE:
				strcpy(ReleaseType,_("Alpha Release"));
				break;
			case TCL_BETA_RELEASE:
				strcpy(ReleaseType,_("Beta Release"));
				break;
			case TCL_FINAL_RELEASE:
				strcpy(ReleaseType,_("Final Release"));
				break;
			default:
				strcpy(ReleaseType,_("Unknown Release"));
				break;
		}

		sprintf(VersionString,"TCL/TK %d.%d.%d %s",major,minor,patchLevel,ReleaseType);
		output=strdup(VersionString);
		n1=1;
		CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
		if (output) {FREE(output);output=NULL;}

		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		if (GetType(1) == sci_strings)
		{
			char *Param=NULL;

			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			Param=cstk(l1);

			if (strcmp(Param,"numbers") == 0)
			{
				int *VERSIONMATRIX=NULL;
				VERSIONMATRIX=(int *)MALLOC( (4)*sizeof(int) );

				VERSIONMATRIX[0]=(int)major;
				VERSIONMATRIX[1]=(int)minor;
				VERSIONMATRIX[2]=(int)patchLevel;
				VERSIONMATRIX[3]=(int)type;

				m1=1;
				n1=4;
				CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &m1, &n1 ,&VERSIONMATRIX);
				LhsVar(1)=Rhs+1;
				PutLhsVar();
				if (VERSIONMATRIX){	FREE(VERSIONMATRIX); VERSIONMATRIX=NULL;}
			}
			else
			{
				Scierror(999,_("%s: Wrong first input argument: '%s' expected.\n"),fname,"numbers");
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument: String expected.\n"),fname);
		}

	}
	return 0;
}
/*--------------------------------------------------------------------------*/
