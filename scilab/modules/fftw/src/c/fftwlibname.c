/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stdlib.h"
#include "fftwlibname.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
char *fftwlibname = NULL;
/*--------------------------------------------------------------------------*/ 
void setfftwlibname(char *libname)
{
	if (libname)
	{
		if (fftwlibname) {FREE(fftwlibname);fftwlibname = NULL;}
		fftwlibname = (char *)MALLOC(sizeof(char)*(strlen(libname)+1));
		strcpy(fftwlibname,libname);
	}
}
/*--------------------------------------------------------------------------*/ 
char *getfftwlibname(void)
{
	char *name = NULL;

	if (fftwlibname)
	{
		name = (char *)MALLOC(sizeof(char)*(strlen(fftwlibname)+1));
		strcpy(name,fftwlibname);
	}

	return name;
}
/*--------------------------------------------------------------------------*/ 
