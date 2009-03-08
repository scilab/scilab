/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>

#include "PATH_MAX.h"
#include "core_math.h"
#include "Os_specific.h"
#include "../../../io/includes/setenvc.h"
#include "cluni0.h"
#include "GetenvB.h"
#include "charEncoding.h"
#include "stack-def.h"

static char *SCI_a[]  = {  "SCI/"   , "sci/"   , "$SCI"   , "SCI\\"    , "sci\\"   , (char *) 0 };
static char *HOME_a[] = {  "HOME/"  , "home/"  , "~/"     , "HOME\\"   , "home\\"  , "~\\" , "$HOME" , (char *) 0};
static char *TMP_a[]  = {  "TMPDIR/", "tmpdir/","TMPDIR\\", "tmpdir\\" , "$TMPDIR" , (char *) 0};

static int Cluni0(char *env,char **alias,char* in_name,char *out_name, long int lin);

static int n=0;

/************************************************
 * expand  in_name to produce out_name
 ************************************************/

int C2F(cluni0)(char *in_name, char *out_name, int *out_n, long int lin, long int lout)
{
	int nc= PATH_MAX;
	static char SCI[PATH_MAX],HOME[PATH_MAX],TMP[PATH_MAX];
	static int k;
	char *in_nameL = NULL;
	char szTemp[bsiz];
    long int linL;
	char in_nameTmp[PATH_MAX];
	strncpy(in_nameTmp,in_name,lin);
	in_nameTmp[lin]='\0';
	/* To convert to UTF-8 */
	in_nameL = UTFToLocale(in_nameTmp, szTemp);
	/* Get the new size of the converted string */
	linL=(long)strlen(in_nameL);
	/* Copy it back into the in_nameTmp variable */
	strncpy(in_nameTmp,in_nameL,linL);
	in_nameL=in_nameTmp;
	if ( ( n==0 ) || (getUpdateEnvVar() == 1) )
	{
		GetenvB("SCI"   , SCI ,nc);
		GetenvB("HOME"  , HOME,nc);
		GetenvB("TMPDIR", TMP ,nc);
		n=n+1;
		setUpdateEnvVar(0);
	}
	
	/* in_name[lin]='\0';*/
	if ( Cluni0(SCI,SCI_a,in_nameL,out_name,linL) == 0 )
		if ( Cluni0(HOME,HOME_a,in_nameL,out_name,linL) == 0 )
			if ( Cluni0(TMP,TMP_a,in_nameL,out_name,linL) == 0 )
			{
				strncpy(out_name,in_nameL,(size_t)linL);
				out_name[linL]='\0';
			}
	
	*out_n = (int)strlen(out_name);
	#ifdef _MSC_VER
		for (k=0 ; k < *out_n ;k++) if ( out_name[k]=='/') out_name[k]='\\';
	#else
		for (k=0 ; k < *out_n ;k++) if ( out_name[k]=='\\') out_name[k]='/';
	#endif
	
	return(0);
}

/**
 * expand in_name to produce out_name
 *     try to find alias[i] at the begining of in_name
 *     and replaces it by env in out_name
 *     out_name must be large enough to get the result
 *
 * @param env
 * @param alias
 * @param in_name
 * @param out_name
 * @param lin
 * @return <ReturnValue>
 */

static int Cluni0(char *env, char **alias, char *in_name, char *out_name, long int lin)
{
	int i=0;
	if ( env[0] == '\0' ) return(0);
	while ( alias[i] != (char *) 0)
	{
		if ( strncmp(alias[i],in_name,strlen(alias[i])) == 0)
		{
			strcpy(out_name,env);
			strncat(out_name,in_name+strlen(alias[i])-1,
			(size_t)(lin-strlen(alias[i]+1)));
			/*sprintf(out_name,"%s/%s",env,in_name+strlen(alias[i]));*/
			return(1);
		}
		i++;
	}
	return(0);
}
