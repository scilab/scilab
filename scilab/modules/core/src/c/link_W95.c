/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "stack-def.h"
#include "warningmode.h"
#include "dynamiclibrary.h"
#include "addinter.h"
/*-----------------------------------------------------------------------------------*/
#ifdef round
#undef round
#endif 

#define round(x,s) (((x) + ((s)-1)) & ~((s)-1))
#define Min(x,y)	(((x)<(y))?(x):(y))
#define Max(x,y)	(((x)>(y))?(x):(y))
#define debug C2F(iop).ddt==1
/*-----------------------------------------------------------------------------------*/
static void Sci_Delsym __PARAMS((int ));
static int Sci_dlopen __PARAMS((char *loaded_files[],int global));
static int Sci_dlsym __PARAMS((char *ename,int  ishared,char * strf));
/*-----------------------------------------------------------------------------------*/
/*************************************
* New version : link entry names 
*   from new shared lib created with 
*   files.
*   -1 : the shared archive was not loaded 
*   -5 : pb with one of the entry point 
*************************************/
void SciLink(int iflag,int *rhs,int *ilib,char *files[],char *en_names[],char *strf)
{
	int i;
	if ( iflag == 0 )
	{
		if ( * rhs == 1 ) 
			/* if no entry names are given we try a dl_open with global option*/
			*ilib  = Sci_dlopen(files,1);
		else 
			*ilib  = Sci_dlopen(files,0);
	}
	if (*ilib  == -1 ) return;
	if ( iflag == 0) 
	{
		if (getWarningMode()) sciprint(_("shared archive loaded\n"));
	}
	if ( *rhs >= 2) 
	{
		i=0 ;
		while ( en_names[i] != (char *) 0)
		{
			if ( Sci_dlsym(en_names[i],*ilib,strf) == FAIL) 
				*ilib=-5;
			i++;
		}
	}
}
/*-----------------------------------------------------------------------------------*/
/**************************************
* return 1 if link accepts multiple file iin one call
* or 0 elsewhere 
*************************************/
int LinkStatus()
{
	return(1);
}

/**************************************
* Unlink a shared lib 
*************************************/

void C2F(isciulink)(integer *i) 
{
	/* delete entry points in shared lib *i */
	Sci_Delsym(*i);
	/* delete entry points used in addinter in shared lib *i */
	RemoveInterf(*i);
}


/*************************************
* This routine 
*   load a shared archive and call dlopen (here LoadLibrary)
*   the shared lib handler is stored in a Table 
*   The return value is == -1 if the dlopen failed 
* 
*************************************/

int Sci_dlopen( char *loaded_files[], int global)
{
	static DynLibHandle  hd1 = NULL;
	int   i;
	int count =0;
	while ( loaded_files[count] != NULL) count++;
	if ( count != 1 ) 
	{
		if (getWarningMode()) sciprint(_("link: first argument must be a unique dll name\n"));
	}
	hd1 =   LoadDynLibrary (loaded_files[0]);
	if ( hd1 == NULL ) 
	{
		if (getWarningMode()) sciprint(_("link failed for dll %s\n"),loaded_files[0]);
		return(-1);
	}
	for ( i = 0 ; i < Nshared ; i++ ) 
	{
		if ( hd[i].ok == FAIL) 
		{
			hd[i].shl =  (unsigned long)hd1;
			//  strcpy(hd[i].tmp_file,loaded_files[i]);
			hd[i].ok = OK;
			return(i);
		}
	}

	if ( Nshared == ENTRYMAX ) 
	{
		if (getWarningMode()) sciprint(_("You can't open shared files maxentry %d reached\n"),ENTRYMAX);
		return(FAIL);
	}

	//strcpy(hd[Nshared].tmp_file,loaded_files[i]);
	hd[Nshared].shl = (unsigned long)hd1;
	hd[Nshared].ok = OK;
	Nshared ++;
	return(Nshared-1);
}


/*************************************
* This routine load the entryname ename 
*     from shared lib ishared 
* return FAIL or OK 
*************************************/

int Sci_dlsym(char *ename,int ishared,char *strf)
{
	DynLibHandle  hd1 = NULL;
	int ish = Min(Max(0,ishared),ENTRYMAX-1);
	char enamebuf[MAXNAME];
	if ( strf[0] == 'f' )
		Underscores(1,ename,enamebuf);
	else 
		Underscores(0,ename,enamebuf);

	/* lookup the address of the function to be called */
	if ( NEpoints == ENTRYMAX ) 
	{
		if (getWarningMode()) sciprint(_("You can't link more functions maxentry %d reached\n"),ENTRYMAX);
		return(FAIL);
	}
	if ( hd[ish].ok == FAIL ) 
	{
		if (getWarningMode()) sciprint(_("Shared lib %d does not exists\n"),ish);
		return(FAIL);
	}
	/** entry was previously loaded **/
	if ( SearchFandS(ename,ish) >= 0 ) 
	{
		if (getWarningMode()) sciprint(_("Entry name %s is already loaded from lib %d\n"),ename,ish);
		return(OK);
	}
	hd1 = (DynLibHandle)  hd[ish].shl;
	EP[NEpoints].epoint = (function) GetDynLibFuncPtr (hd1,enamebuf);
	if ( EP[NEpoints].epoint == NULL )
	{
		if (getWarningMode()) sciprint(_("%s is not an entry point\n"),enamebuf);
		return(FAIL);
	}
	else 
	{
		/* we don't add the _ in the table */
		if (debug) sciprint(_("Linking %s\n"),ename);
		strncpy(EP[NEpoints].name,ename,MAXNAME);
		EP[NEpoints].Nshared = ish;
		NEpoints++;
	}
	return(OK);  
}

/***************************************************
* Delete entry points associated with shared lib ishared
* then delete the shared lib 
****************************************************/

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
	if ( hd[ish].ok != FAIL)
	{
		FreeDynLibrary ((DynLibHandle) hd[ish].shl);
		hd[ish].ok = FAIL;
	}
}

