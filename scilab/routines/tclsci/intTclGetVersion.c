/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGetVersion.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGetVersion) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int major=0;
	int minor=0;
	int patchLevel=0;
	int type=0;
	char *output=NULL ;
	char VersionString[256];
	char ReleaseType[256];

	CheckRhs(0,0);
	CheckLhs(1,1);
	
	Tcl_GetVersion(&major, &minor, &patchLevel, &type);
	switch (type)
	{
		case TCL_ALPHA_RELEASE:
			sprintf(ReleaseType,TCL_MSG2);
			break;
		case TCL_BETA_RELEASE:
			sprintf(ReleaseType,TCL_MSG3);
			break;
		case TCL_FINAL_RELEASE:
			sprintf(ReleaseType,TCL_MSG4);
			break;
		default:
			sprintf(ReleaseType,TCL_MSG5);
			break;
	}

	sprintf(VersionString,TCL_MSG6,major,minor,patchLevel,ReleaseType);
	
	output=(char*)MALLOC((strlen(VersionString)+1)*sizeof(char));
	sprintf(output,"%s",VersionString);
	n1=1;
	CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);
	if (output) {FREE(output);output=NULL;}
			
	LhsVar(1) = 1;
	C2F(putlhsvar)();

	return 0;
}
/*-----------------------------------------------------------------------------------*/
