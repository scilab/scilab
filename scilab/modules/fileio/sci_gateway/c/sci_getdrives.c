/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h" 
#include "getdrives.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_getdrives) _PARAMS((char *fname,unsigned long l))
{	
	static int l1=0,n1=0,m1=0;
	char **Drives=NULL;
	char DrvLetter[4] = "A:\\";
	int nbDrives=0;
	int i=0;

	Rhs=Max(Rhs,0);
	CheckRhs(0,0);
	CheckLhs(0,1);

	Drives=getdrives(&nbDrives);

	if (Drives)
	{
		m1=1;
		n1=nbDrives;
		CreateVarFromPtr(Rhs+1,"S", &m1, &n1,Drives);
	}
	else
	{
		m1=0; n1=0; l1=0; 
		/* returns a [] */
		CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
	}
		
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();	

	if (Drives)
	{
		for (i=0;i<nbDrives;i++)
		{
			if (Drives[i]) {FREE(Drives[i]);Drives[i]=NULL;}
		}
		FREE(Drives);
		Drives=NULL;
		nbDrives=0;
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
