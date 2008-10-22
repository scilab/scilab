/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA/ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*---------------------------------------------------------------------------*/
#include <string.h> 
#include <stdio.h>
#include <stdlib.h>
#include "dynamic_link.h"
#include "dynamiclibrary.h"
#include "../../../string/includes/men_Sutils.h"
#include "MALLOC.h" /* MALLOC */
#include "warningmode.h"
#include "sciprint.h"
#include "stack-c.h"
#include "addinter.h"
#include "localization.h"
#include "Scierror.h"
#include "FileExist.h"
#ifdef _MSC_VER
#include "getenvc.h"
#endif
/*---------------------------------------------------------------------------*/
static void Underscores(int isfor, char *ename, char *ename1);
static int SearchFandS(char *op, int ilib);
/*---------------------------------------------------------------------------*/
#define MAXNAME  256 
#define TMPL 256
#define debug C2F(iop).ddt==1
/*---------------------------------------------------------------------------*/
#ifdef _MSC_VER
/* struct used by fortran (F2C) */
/* required to be defined in C */
typedef struct {
	char name[nlgh+1];
} CINTER_struct;

__declspec (dllexport) CINTER_struct C2F(cinter);

/* struct used by fortran (F2C) */
/* required to be defined in C */
typedef struct {
	int ibuf[lsiz];
} IBFU_struct;
__declspec (dllexport) CINTER_struct C2F(ibfu);

#endif
/*---------------------------------------------------------------------------*/
typedef char Name[MAXNAME];   /* could be changed to dynamic structure */

typedef void (*function) ();

typedef struct 
{ 
	function epoint;            /* the entry point */ 
	Name     name;              /* entry point name */
	int      Nshared;           /* number of the shared file */
} Epoints;

typedef struct 
{
	int ok;
	char tmp_file[TMPL];
	unsigned long  shl;
} Hd;

static Hd  hd[ENTRYMAX]; /* shared libs handler */
static int Nshared = 0;
static int NEpoints = 0; /* Number of Linked names */
static Epoints EP[ENTRYMAX];  /* entryPoints */
/*---------------------------------------------------------------------------*/
int scilabLink(int idsharedlibrary,
			   char *filename,
			   char **subnamesarray,int sizesubnamesarray,
			   BOOL fflag,int *ierr)
{
	int IdSharedLib = -1; 

	initializeLink();

	if (idsharedlibrary == -1) {
		IdSharedLib= Sci_dlopen(filename);
	} else {
		IdSharedLib = idsharedlibrary;
	}

	if (IdSharedLib == -1 ) 
	{
		if ( getWarningMode() ) 
		{
			#ifdef _MSC_VER
			{
				char *pathSearch = searchEnv(filename,"PATH");
				if (pathSearch)
				{
					sciprint(_("%s: The file %s does not exist.\n" ),"link",filename);
					FREE(pathSearch);
				}
			}
			#else
			sciprint(_("Link failed for dynamic library '%s'.\n"),filename);
			sciprint(_("An error occurred: %s\n"),GetLastDynLibError());
			#endif
		}
		*ierr = -1;
		return IdSharedLib;
	}
	if ( (idsharedlibrary == -1) && getWarningMode() ) 
	{
		sciprint(_("Shared archive loaded.\n"));
		sciprint(_("Link done.\n"));
	}

	if (sizesubnamesarray > 0)
	{
		int errorcode = 0;
		int i = 0;
		for(i = 0; i < sizesubnamesarray ; i++)
		{
			if (fflag) {
				errorcode = Sci_dlsym(subnamesarray[i],IdSharedLib,"f");
			}else {
				errorcode = Sci_dlsym(subnamesarray[i],IdSharedLib,"c");
			}
			
			if (errorcode < 0) *ierr = errorcode;
		}
	}
	return IdSharedLib;
}
/*---------------------------------------------------------------------------*/
int *getAllIdSharedLib(int *sizeList)
{
	int *ListId = NULL;
	int i = 0;

	*sizeList = 0;
	for ( i = 0 ; i < Nshared ; i++) 
	{
		if ( hd[i].ok == TRUE) 
		{
			(*sizeList)++;
			if (ListId)
			{
				ListId = (int *)REALLOC(ListId,(*sizeList)*sizeof(int));
				ListId[(*sizeList)-1] = i;
			}
			else
			{
				ListId = (int *)MALLOC((*sizeList)*sizeof(int));
				ListId[(*sizeList)-1] = i;
			}
		}
	}
	return ListId;
}
/*---------------------------------------------------------------------------*/
char **getNamesOfFunctionsInSharedLibraries(int *sizearray)
{
	char **NamesOfFunctions = NULL;
	*sizearray = 0;

	if ( (NEpoints) && (NEpoints > 0) )
	{
		int i = 0;
		NamesOfFunctions = (char **) MALLOC((NEpoints)*sizeof(char *));
		if (NamesOfFunctions)
		{
			for ( i = NEpoints-1 ; i >= 0 ; i--) 
			{
				if (EP[i].name)
				{
					char *EntryName = (char *)MALLOC(((int)strlen(EP[i].name)+1)*sizeof(char));
					
					if (EntryName)
					{
						(*sizearray)++;
						strcpy(EntryName , EP[i].name);
						NamesOfFunctions[(*sizearray)-1] = EntryName;
					}
				}
			}
		}
	}
	return NamesOfFunctions;
}
/*---------------------------------------------------------------------------*/
/**
* Underscores : deals with the trailing _ 
* in entry names 
*/
static void Underscores(int isfor, char *ename, char *ename1)
{
#ifdef WLU1
	*ename1='_'; ename1++;
#endif
	strcpy(ename1,ename);
#ifdef WTU
	if (isfor==1) strcat(ename1,"_");
#endif
	return;
}
/*---------------------------------------------------------------------------*/
void initializeLink(void)
{
	static int first_entry = 0;
	int i;
	if ( first_entry == 0)
	{
		for ( i = 0 ; i < ENTRYMAX ; i++) 
		{
			hd[i].ok= FALSE;
			hd[i].shl = EP[i].Nshared = -1;
		}
		first_entry++;
	}
}
/*---------------------------------------------------------------------------*/
BOOL c_link(char *routinename,int *ilib)
{
	void (*loc)();
	if ( *ilib != -1 ) *ilib = SearchFandS(routinename,*ilib);
	else *ilib = SearchInDynLinks(routinename,&loc);

	if (*ilib == -1) return FALSE;
	return TRUE;
}
/*---------------------------------------------------------------------------*/
void C2F(iislink)(char *routinename, int *ilib)
{
	c_link(routinename,ilib);
}
/*---------------------------------------------------------------------------*/
void GetDynFunc(int ii, void (**realop) ())
{
	if ( EP[ii].Nshared != -1 ) *realop = EP[ii].epoint;
	else *realop = (function) 0;
}
/*---------------------------------------------------------------------------*/
int SearchInDynLinks(char *op, void (**realop) ())
{
	int i=0;
	for ( i = NEpoints-1 ; i >=0 ; i--) 
	{
		if ( strcmp(op,EP[i].name) == 0) 
		{
			*realop = EP[i].epoint;
			return(EP[i].Nshared );
		}
	}
	return(-1);
}
/*---------------------------------------------------------------------------*/
/**
* Search a (function,libid) in the table 
* Search from end to top 
*/
static int SearchFandS(char *op, int ilib)
{
	int i = 0;
	for ( i = NEpoints-1 ; i >=0 ; i--) 
	{
		if ( strcmp(op,EP[i].name) == 0 && EP[i].Nshared == ilib)
		{
			return(i);
		}
	}
	return(-1);
}
/*---------------------------------------------------------------------------*/
void ShowDynLinks(void)
{
	int i=0,count=0;
	if (getWarningMode()) sciprint(_("Number of entry points %d.\nShared libraries :\n"),NEpoints);
	if (getWarningMode())sciprint("[ ");
	for ( i = 0 ; i < Nshared ; i++) 
	{
		if ( hd[i].ok == TRUE) { if (getWarningMode())sciprint("%d ",i);count++;}
	}
	if (getWarningMode()) sciprint(_("] : %d libraries.\n"),count);

	for ( i = NEpoints-1 ; i >=0 ; i--) 
	{
		if (getWarningMode()) sciprint(_("Entry point %s in shared library %d.\n"),EP[i].name,EP[i].Nshared);
	}
}
/*---------------------------------------------------------------------------*/
void unlinkallsharedlib(void)
{
	int i=0;
	for ( i = 0 ; i < Nshared ; i++) 
	{
		unlinksharedlib(&i);
	}
}
/*---------------------------------------------------------------------------*/
void unlinksharedlib(int *i) 
{
	/* delete entry points in shared lib *i */
	Sci_Delsym(*i);
	/* delete entry points used in addinter in shared lib *i */
	RemoveInterf(*i);
}
/*---------------------------------------------------------------------------*/
int Sci_dlopen( char *loaded_file)
{
	static DynLibHandle  hd1 = NULL;
	int i = 0;

	hd1 = LoadDynLibrary (loaded_file);

	if ( hd1 == NULL )  return -1 ; /* the shared archive was not loaded. */

	for ( i = 0 ; i < Nshared ; i++ ) 
	{
		if ( hd[i].ok == FALSE) 
		{
			/* Warning x64 windows */
			#ifdef _MSC_VER
			hd[i].shl =  PtrToUlong(hd1);
			#else
			hd[i].shl = (unsigned long)hd1;
			#endif
			
			hd[i].ok = TRUE;
			return(i);
		}
	}

	if ( Nshared == ENTRYMAX ) 
	{
		if (getWarningMode()) sciprint(_("Cannot open shared files max entry %d reached.\n"),ENTRYMAX);
		return(FALSE);
	}
	/* Warning x64 windows */
	#ifdef _MSC_VER
	hd[Nshared].shl =   PtrToUlong(hd1);
	#else
	hd[Nshared].shl = (unsigned long)hd1;
	#endif
	hd[Nshared].ok = TRUE;
	Nshared ++;

	return Nshared-1;
}
/*---------------------------------------------------------------------------*/
int Sci_dlsym(char *ename,int ishared,char *strf)
{
	DynLibHandle hd1 = NULL;
	int ish = Min(Max(0,ishared),ENTRYMAX-1);
	char enamebuf[MAXNAME];

	if ( strf[0] == 'f' ) Underscores(1,ename,enamebuf);
	else Underscores(0,ename,enamebuf);

	/* lookup the address of the function to be called */
	if ( NEpoints == ENTRYMAX ) 
	{
		return -1;
	}
	if ( hd[ish].ok == FALSE )
	{
		return -3;
	}
	/** entry was previously loaded **/
	if ( SearchFandS(ename,ish) >= 0 ) 
	{
		sciprint(_("Entry name %s.\n"),ename);
		return -4;
	}
	else
	{
		/* Warning x64 windows */
		#ifdef _MSC_VER
		hd1 = (DynLibHandle)  ULongToHandle(hd[ish].shl);
		#else
		hd1 = (DynLibHandle)  hd[ish].shl;
		#endif
		EP[NEpoints].epoint = (function) GetDynLibFuncPtr (hd1,enamebuf);
		if ( EP[NEpoints].epoint == NULL )
		{
			if (getWarningMode()) sciprint(_("%s is not an entry point.\n"),enamebuf);
			return -5;
		}
		else 
		{
			/* we don't add the _ in the table */
			if (debug) sciprint(_("Linking %s.\n"),ename);
			strncpy(EP[NEpoints].name,ename,MAXNAME);
			EP[NEpoints].Nshared = ish;
			NEpoints++;
		}
	}
	return 0;  
}
/*---------------------------------------------------------------------------*/
void Sci_Delsym(int ishared) 
{
	int ish = Min(Max(0,ishared),ENTRYMAX-1);
	int i=0;
	for ( i = NEpoints-1 ; i >=0 ; i--) 
	{
		if ( EP[i].Nshared == ish )
		{
			int j;
			for ( j = i ; j <= NEpoints - 2 ; j++ )
			{
				EP[j].epoint = EP[j+1].epoint;
				EP[j].Nshared = EP[j+1].Nshared;
				strcpy(EP[j].name,EP[j+1].name);
			}
			NEpoints--;
		}
	}
	if ( hd[ish].ok != FALSE)
	{
		/* Warning x64 windows */
		#ifdef _MSC_VER
		FreeDynLibrary ((DynLibHandle) ULongToHandle(hd[ish].shl));
		#else
		FreeDynLibrary ((DynLibHandle) hd[ish].shl);
		#endif
		hd[ish].ok = FALSE;
	}
}
/*---------------------------------------------------------------------------*/
