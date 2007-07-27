/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "librarieslist.h"
#include "stackinfo.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
char **getlibrarieslist(int *sizearray)
{
	char **librarieslist = NULL;
	int Lused=0;
	int Ltotal=0;
	int j = 0;
	int nbElements=0;

	C2F(getvariablesinfo)(&Ltotal,&Lused);

	for (j=1;j<Lused+1;++j)
	{
		char *NameVariable = getLocalNamefromId(j);
		int lw; int fin;
		
		if (C2F(objptr)(NameVariable,&lw,&fin,strlen(NameVariable))) 
		{
			int *header = istk( iadr(*Lstk(fin)));  
			if ( (header) && (header[0] == sci_lib ) ) 
			{
				nbElements++;
				if (librarieslist) librarieslist=(char**)REALLOC(librarieslist,sizeof(char*)*(nbElements));
				else librarieslist = (char**)MALLOC(sizeof(char*)*(nbElements));
				librarieslist[nbElements-1] = NameVariable;
			}
		}
	}

	*sizearray = nbElements;
	return librarieslist;
}
/*-----------------------------------------------------------------------------------*/
