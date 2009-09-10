/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#ifdef HAVE_LIMITS_H
#include <limits.h>
#define MAXLONG LONG_MAX
#else 
#ifdef HAVE_VALUES_H
#include <values.h>
#endif /* HAVE_VALUES_H */
#endif /* !HAVE_LIMITS_H */
#include "stackinfo.h"
#include "stack-def.h"
#include "stack-c.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif

#ifndef LONG_MAX 
#define LONG_MAX 2147483647L 
#endif 

#ifndef MAXLONG
#define MAXLONG LONG_MAX
#endif
/*--------------------------------------------------------------------------*/
static void cleanFortranString(char *fortanbuffer);
/*--------------------------------------------------------------------------*/
int C2F(getstackinfo)(int *total,int *used)
{
	*used = C2F(vstk).lstk[C2F(vstk).isiz - 1] - C2F(vstk).lstk[Bot - 1] + 1;
	*total = C2F(vstk).lstk[C2F(vstk).isiz - 1] - C2F(vstk).lstk[0];
	return(0);
}
/*--------------------------------------------------------------------------*/
int C2F(getgstackinfo)(int *total,int *used)
{
	*used = C2F(vstk).lstk[C2F(vstk).gtop] - C2F(vstk).lstk[C2F(vstk).isiz + 1] + 1;
	*total = C2F(vstk).lstk[C2F(vstk).gbot - 1] - C2F(vstk).lstk[C2F(vstk).isiz + 1] ;
	return(0);
}
/*--------------------------------------------------------------------------*/
int C2F(getvariablesinfo)(int *total,int *used)
{
	*used = C2F(vstk).isiz - Bot ;
	*total = C2F(vstk).isiz - 1;
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(getgvariablesinfo)(int *total,int *used)
{
	*used = C2F(vstk).gtop - C2F(vstk).isiz - 1;
	*total = 10000 - C2F(vstk).isiz - 1;
	return 0;
}
/*--------------------------------------------------------------------------*/
int getIntermediateMemoryNeeded(void)
{
	return (Err + C2F(vstk).lstk[Bot - 1] - C2F(vstk).lstk[0]);
}
/*--------------------------------------------------------------------------*/
BOOL is_a_valid_size_for_scilab_stack(int sizestack)
{
#if defined(_MSC_VER) && defined(_WIN64)
	/* On Windows x64 with scilab's stack , we need to limite stack access */
	if ((unsigned long)sizestack >= get_max_memory_for_scilab_stack()+1)
	{
		return FALSE;
	}
	return TRUE;
#else
	double dsize = ((double) sizeof(double)) * (sizestack);
	unsigned long ulsize = ((unsigned long)sizeof(double)) * (sizestack);
	if ( dsize != (double) ulsize)
	{
		return FALSE;
	}
	return TRUE;
#endif
}
/*--------------------------------------------------------------------------*/
unsigned long get_max_memory_for_scilab_stack(void)
{
#if defined(_MSC_VER) && defined(_WIN64)
	return MAXLONG32/sizeof(double);
#else
	return MAXLONG/sizeof(double);
#endif
}
/*--------------------------------------------------------------------------*/
char *getLocalNamefromId(int n)
{
	int *id=NULL;	
	int one = 1;
	char *Name=NULL;
	char fortranName[nlgh+1];

	id=&C2F(vstk).idstk[Bot * 6 - 6];
	id -= 7;

	C2F(cvname)(&id[n * 6 + 1], fortranName, &one,nlgh);

	cleanFortranString(fortranName);

	if (strlen(fortranName) > 0)
	{
		Name = strdup(fortranName);
	}
	return Name;
}
/*--------------------------------------------------------------------------*/
char *getGlobalNamefromId(int n)
{
	int *id=NULL;	
	static int one = 1;
	char *Name=NULL;
	char fortranName[nlgh+1];

	id=&C2F(vstk).idstk[(C2F(vstk).isiz + 2) * 6 - 6];
	id -= 7;

	C2F(cvname)(&id[(n+1) * 6 + 1], fortranName, &one,nlgh);

	cleanFortranString(fortranName);

	if (strlen(fortranName) > 0)
	{
		Name = strdup(fortranName);
	}
	return Name;
}
/*--------------------------------------------------------------------------*/
int getLocalSizefromId(int n)
{
	int LocalSize=0;
	int Lused=0;
	int Ltotal=0;

	C2F(getvariablesinfo)(&Ltotal,&Lused);

	if ( (n >= 0) && ( n < Lused ) )
	{
		LocalSize=(int)(C2F(vstk).lstk[Bot + n] - C2F(vstk).lstk[Bot + n - 1]);
	}
	else
	{
		LocalSize=-1;
	}

	return LocalSize;
}
/*--------------------------------------------------------------------------*/
int getGlobalSizefromId(int n)
{
	int GlobalSize=0;
	int Gused=0;
	int Gtotal=0;

	C2F(getgvariablesinfo)(&Gtotal,&Gused);

	if ( (n >= 0) && ( n < Gused ) )
	{
		GlobalSize=(int)(C2F(vstk).lstk[C2F(vstk).isiz + 2 + n] - C2F(vstk).lstk[C2F(vstk).isiz + 2 + n - 1]);
	}
	else
	{
		GlobalSize=-1;
	}
	return GlobalSize;
}
/*--------------------------------------------------------------------------*/
static void cleanFortranString(char *fortanbuffer)
{
	int i = 0;
	fortanbuffer[nlgh]='\0';

	for (i=0;i < nlgh;i++)
	{
		if (fortanbuffer[i] == '\0')
		{
			break;
		}
		else if (fortanbuffer[i] == ' ')
		{
			fortanbuffer[i] = '\0';
			break;
		}
	}
}
/*--------------------------------------------------------------------------*/
BOOL existVariableNamedOnStack(char *varname)
{
	if (existLocalVariableNamedOnStack(varname) || 
		existGlobalVariableNamedOnStack(varname) )
	{
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL existLocalVariableNamedOnStack(char *varname)
{
	if (varname)
	{
		int Lused = 0;
		int Ltotal = 0;
		int i = 0;

		C2F(getvariablesinfo)(&Ltotal,&Lused);

		for( i = 0; i < Lused; i++)
		{
			char *varOnStack = getLocalNamefromId(i);
			if (varOnStack)
			{
				if (strcmp(varname, varOnStack) == 0)
				{
					FREE(varOnStack);
					varOnStack = NULL;
					return TRUE;
				}
				FREE(varOnStack);
				varOnStack = NULL;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL existGlobalVariableNamedOnStack(char *varname)
{
	if (varname)
	{
		int Gused = 0;
		int Gtotal = 0;
		int i = 0;

		C2F(getgvariablesinfo)(&Gtotal,&Gused);
		for( i = 0; i < Gused; i++)
		{
			char *varOnStack = getGlobalNamefromId(i);
			if (varOnStack)
			{
				if (strcmp(varname, varOnStack) == 0)
				{
					FREE(varOnStack);
					varOnStack = NULL;
					return TRUE;
				}
				FREE(varOnStack);
				varOnStack = NULL;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
