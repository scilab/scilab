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
int C2F(sci_TCL_GetVersion) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int major=0;
	int minor=0;
	int patchLevel=0;
	int type=0;
	char *output=NULL ;
	char VersionString[256];
	char ReleaseType[256];
	char *msg_=NULL;

	CheckRhs(0,1);
	CheckLhs(1,1);

	Tcl_GetVersion(&major, &minor, &patchLevel, &type);

	if (Rhs == 0)
	{
		switch (type)
		{
		case TCL_ALPHA_RELEASE:
			{
				char *msg=_("Alpha Release");
				sprintf(ReleaseType,msg);
				if (msg) {FREE(msg);msg=NULL;}
			}
			break;
		case TCL_BETA_RELEASE:
			{
				char *msg=_("Beta Release");
				sprintf(ReleaseType,msg);
				if (msg) {FREE(msg);msg=NULL;}
			}
			break;
		case TCL_FINAL_RELEASE:
			{
				char *msg=_("Final Release");
				sprintf(ReleaseType,msg);
				if (msg) {FREE(msg);msg=NULL;}
			}
			break;
		default:
			{
				char *msg=_("Unknown Release");
				sprintf(ReleaseType,msg);
				if (msg) {FREE(msg);msg=NULL;}
			}
			break;
		}

		msg_=_("TCL/TK %d.%d.%d %s");
		sprintf(VersionString,msg_,major,minor,patchLevel,ReleaseType);
		if (msg_) {FREE(msg_);msg_=NULL;}

		output=(char*)MALLOC((strlen(VersionString)+1)*sizeof(char));
		strcpy(output,VersionString);
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
				Scierror(999,_("%s: invalid output argument : help TCL_GetVersion.\n"),fname);
			}
		}
		else
		{
			Scierror(999,_("%s: invalid input argument : help TCL_GetVersion.\n"),fname);
		}
		
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
