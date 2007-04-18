/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "findfiles.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_findfiles) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *path=NULL;
	char *filespec=NULL;
	char **FilesList=NULL;
	int sizeListReturned=0;

	Rhs=Max(Rhs,0);
	CheckRhs(0,2) ;
	CheckLhs(0,1) ;

	switch(Rhs)
	{
		case 0:
		{

		}
		break;

		case 1:
		{

		}
		break;

		case 2:
		{

		}
		break;

		default:
		break;
	}

	FilesList=findfiles(path,filespec,&sizeListReturned);

	if (FilesList)
	{
		LhsVar(1) = 0;
	}
	else
	{
		n1=0;m1=0;l1=0;
		CreateVarFromPtr(Rhs+ 1, "d",&n1,&m1,&l1);
		LhsVar(1) = Rhs+1;
	}

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/