/*
 * Provides is_dir to scilab
 * @author Sylvestre LEDRU - INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_io.h"
#include "directories.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "cluni0.h"
#include "Scierror.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_isdir) _PARAMS((char *fname,unsigned long fname_len))
{
	int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if (! (GetType(1) == sci_strings))
	{
		Scierror(999,"parameter incorrect must be a string (a file pathname).\r\n");
		return 0;
	}
	else
	{
		char *path=NULL, *myPath=NULL;
		char filename[FILENAME_MAX];
		long int lout;
		int out_n;
		BOOL result=FALSE;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		if ( n1==1 )
		{
			path=cstk(l1);
		}
		else
		{
			Scierror(999,"First parameter incorrect, must be a string (a file pathname).\r\n");
			return 0;
		}

		/* make sure the names are not too long */
		
		if( strlen(path) > MAX_PATH_LONG )
		{
			Scierror(999,"The parameter is too long : must be less than %d caracters.\r\n",MAX_PATH_LONG);
		}
		
		/* Crappy workaround because a / was added after SCI & ~ into 
		 * the Scilab macros
		 * cluni0 waits for SCI/ or ~/. It doesn't detect isdir("SCI")
		 */
		if(strcmp(path,"SCI")==0)
		{
			myPath=MALLOC((m1+2)*sizeof(char)); /* +2 because the / added + \0 */
			strcpy(myPath,"SCI/");
		}
		if(strcmp(path,"~")==0)
		{
			myPath=MALLOC((m1+2)*sizeof(char)); /* +2 because the / added + \0 */
			strcpy(myPath,"~/");
		}
		/* End of the crappy workaround */

		lout=FILENAME_MAX;
		if(myPath==NULL)
		{
			/* Replaces SCI, ~, HOME by the real path */
			C2F(cluni0)(path, filename, &out_n,m1*n1,lout);
		}
		else
		{
			/* Replaces SCI, ~, HOME by the real path */
			C2F(cluni0)(myPath, filename, &out_n,m1*n1,lout);
		}

		result=isdir(filename);
		m1=1;
		n1=1;
		CreateVar(Rhs+1, "b", &m1, &n1 ,&l1); /* Create the space in the stack for result */
		*istk(l1)=result; /* Copy comm_size into the stack */
		
		LhsVar(1)= Rhs+1;
		C2F(putlhsvar)();
		if (myPath) {FREE(myPath);myPath=NULL;}
	}
	return 0;
	
}
/*-----------------------------------------------------------------------------------*/
