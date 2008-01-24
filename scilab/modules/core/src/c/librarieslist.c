/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "librarieslist.h"
#include "stackinfo.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
static int getnumberoflibraries(void);
/*--------------------------------------------------------------------------*/
char **getlibrarieslist(int *sizearray)
{
	char **librarieslist = NULL;

	if (librarieslist)
	{
		int Lused=0; int Ltotal=0;
		int j = 0; int i = 0;
		int lw = 0;	int fin = 0;

		C2F(getvariablesinfo)(&Ltotal,&Lused);

		for (j=1;j<Lused+1;++j)
		{
			char *NameVariable = getLocalNamefromId(j);
			if (C2F(objptr)(NameVariable,&lw,&fin,(unsigned long)strlen(NameVariable))) 
			{
				int *header = istk( iadr(*Lstk(fin)));
				if ( (header) && (header[0] == sci_lib ) ) 
				{

					librarieslist[i]= (char*)MALLOC(sizeof(char)*(strlen(NameVariable)+1));
					strcpy(librarieslist[i],NameVariable);
					i++;
				}
			}
			if (NameVariable) {FREE(NameVariable);NameVariable = NULL;}
		}
		*sizearray = i;
	}
	else
	{
		*sizearray = 0;
	}
	return librarieslist;
}
/*--------------------------------------------------------------------------*/
static int getnumberoflibraries(void)
{
	int nbrlibraries = 0;
	int Lused=0;
	int Ltotal=0;
	int j = 0;

	C2F(getvariablesinfo)(&Ltotal,&Lused);

	for (j=1;j<Lused+1;++j)
	{
		int lw = 0; int fin = 0;
		char *NameVariable = getLocalNamefromId(j);
		
		if (C2F(objptr)(NameVariable,&lw,&fin,(unsigned long)strlen(NameVariable))) 
		{
			int *header = istk( iadr(*Lstk(fin)));  
			if ( (header) && (header[0] == sci_lib ) )  nbrlibraries++;
		}
		if (NameVariable) {FREE(NameVariable);NameVariable = NULL;}
	}
	return nbrlibraries;
}
/*--------------------------------------------------------------------------*/
