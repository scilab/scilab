/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "findfiles.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "../../../core/src/c/scicurdir.h" /* C2F(scigetcwd) */
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
#define DEFAULT_FILESPEC "*.*"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_findfiles) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	char *path=NULL;
	char *filespec=NULL;
	char **FilesList=NULL;
	int sizeListReturned=0;
	BOOL needtofreefilespec = FALSE;
	BOOL needtofreepath = FALSE;

	Rhs=Max(Rhs,0);
	CheckRhs(0,2) ;
	CheckLhs(0,1) ;

	switch(Rhs)
	{
	default: case 0:
		{
			int ierr=0;
			int lpath=0;

			C2F(scigetcwd)(&path,&lpath,&ierr);
			needtofreepath = TRUE;

			if (ierr)
			{
				Scierror(999,_("Problem with current directory.\n"));
				return 0;
			}
			else
			{
				filespec=(char *)MALLOC(sizeof(char)*(strlen(DEFAULT_FILESPEC)+1));
				strcpy(filespec,DEFAULT_FILESPEC);
				needtofreefilespec = TRUE;
			}
		}
		break;

	case 1:
		{
			if (GetType(1) == sci_strings)
			{
				GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
				path=cstk(l1);
				needtofreepath = FALSE;

				filespec=(char *)MALLOC(sizeof(char)*(strlen(DEFAULT_FILESPEC)+1));
				strcpy(filespec,DEFAULT_FILESPEC);
				needtofreefilespec = TRUE;
			}
			else
			{
				Scierror(999,_("Invalid parameter , it must be a path (string).\n"));
				return 0;
			}

		}
		break;

	case 2:
		{
			if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
			{
				GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
				path=cstk(l1);
				needtofreepath = FALSE;

				GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
				filespec=cstk(l1);
				needtofreefilespec = FALSE;
			}
			else
			{
				Scierror(999,_("Invalid parameter(s) , it must be a path and a filespec (string).\n"));
				return 0;
			}
		}
		break;
	}

	FilesList=findfiles(path,filespec,&sizeListReturned);
	if ( (filespec) && needtofreefilespec ) {FREE(filespec);filespec = NULL;}
	if ( (path) && needtofreepath ) {FREE(path);path=NULL;}

	if (FilesList)
	{
		int ncol,nrow;

		ncol=1;
		nrow=sizeListReturned;

		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nrow, &ncol, FilesList);
		LhsVar(1) = Rhs+1;
	}
	else
	{
		n1=0;m1=0;l1=0;
		CreateVarFromPtr(Rhs+ 1,MATRIX_OF_DOUBLE_DATATYPE,&n1,&m1,&l1);
		LhsVar(1) = Rhs+1;
	}

	if (FilesList)
	{
		int i=0;
		for (i=0;i<sizeListReturned;i++)
		{
			if (FilesList[i])
			{
				FREE(FilesList[i]);
				FilesList[i]=NULL;
			}
		}
		FREE(FilesList);
		FilesList=NULL;
	}

	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
